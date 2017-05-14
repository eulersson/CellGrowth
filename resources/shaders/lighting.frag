#version 410 core

////////////////////////////////////////////////////////////////////////////////
/// Inputs & Outputs
////////////////////////////////////////////////////////////////////////////////
out vec4 fColor;
in vec2 vTexCoords;

////////////////////////////////////////////////////////////////////////////////
/// Local structures
////////////////////////////////////////////////////////////////////////////////
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    float attenuation;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float Linear;
    float Quadratic;
};

struct FillLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

////////////////////////////////////////////////////////////////////////////////
/// Textures
////////////////////////////////////////////////////////////////////////////////
uniform sampler2D tWorldPosition;   //Real position in 3D space
uniform sampler2D tWorldNormal;
uniform sampler2D tSSAO;
uniform samplerCube tSkyBox; // Cubemap


////////////////////////////////////////////////////////////////////////////////
/// Uniforms
////////////////////////////////////////////////////////////////////////////////
uniform Material material;
uniform Light light;
uniform FillLight fillLight;
uniform bool drawLinks;

////////////////////////////////////////////////////////////////////////////////
///Matricies
////////////////////////////////////////////////////////////////////////////////
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
mat3 normalMatrix = transpose(inverse(mat3(ViewMatrix * ModelMatrix)));



////////////////////////////////////////////////////////////////////////////////
/// Globals
////////////////////////////////////////////////////////////////////////////////
vec3 WorldPosition = texture(tWorldPosition, vTexCoords).xyz;
vec3 ViewPosition  = vec3(ViewMatrix * ModelMatrix * vec4(WorldPosition, 1.0)).xyz;
vec3 WorldNormal = normalize(texture(tWorldNormal, vTexCoords).xyz);
vec3 ViewNormal  = normalize(normalMatrix * WorldNormal);
float Occlusion = texture(tSSAO, vTexCoords).r;
vec4 SkyBox = texture(tSkyBox, WorldNormal);


////////////////////////////////////////////////////////////////////////////////
/// Subroutine definition
////////////////////////////////////////////////////////////////////////////////
subroutine vec4 RenderType();
subroutine uniform RenderType RenderTypeSelection;


////////////////////////////////////////////////////////////////////////////////
/// ADS SHADING
///
/// Source:
/// Anon, n.d.  Learn OpenGL, extensive tutorial resource for learning Modern OpenGL.
/// [online] Learnopengl.com. Available from:
/// https://learnopengl.com/#!Lighting/Basic-Lighting [Accessed 21 Feb. 2017].
///
/// Anon, n.d. Learn OpenGL, extensive tutorial resource for learning Modern OpenGL.
/// [online] Learnopengl.com. Available from:
/// https://learnopengl.com/#!Lighting/Materials [Accessed 21 Feb. 2017].
////////////////////////////////////////////////////////////////////////////////
subroutine (RenderType)
vec4 ADSRender()
{
    vec3 lightDir = normalize(light.position - WorldPosition) + normalize(fillLight.position - WorldPosition);
    vec3 viewDir  = normalize(-WorldPosition);

    // Ambient
    vec3 ambient = (material.ambient * light.ambient) + (material.ambient * fillLight.ambient);

    // Diffuse
    vec3 diffuse = max(dot(WorldNormal, lightDir), 0.0) * ((material.diffuse * light.diffuse) + (material.diffuse * fillLight.diffuse));

    // Specular
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(WorldNormal, halfwayDir), 0.0), 8.0);
    vec3 specular = (spec * light.specular) + (spec * fillLight.specular * 0.5);

    // Attenuation
    float distance = length(light.position - WorldPosition);
    float attenuation = 1.0 / (1.0 + light.Linear * distance + light.Quadratic * distance * distance);
    diffuse *= attenuation;
    specular *= attenuation;
    
    vec3 lighting = (ambient + diffuse + specular);

    lighting = clamp(lighting, vec3(0), vec3(1));

  return vec4(lighting, 1.0);
}

////////////////////////////////////////////////////////////////////////////////
/// X-RAY SHADING
/// Source: https://machinesdontcare.wordpress.com/2009/06/09/simple-x-ray-shader
/// Source: http://www.tinysg.de/techGuides/tg2_xray.html
////////////////////////////////////////////////////////////////////////////////
subroutine (RenderType)
vec4 XRayRender()
{
    vec4 result = vec4(1.0, 1.0, 1.0, 1.0);
    
    float EdgeFalloff = 2.3;
    float opacity = dot(normalize(ViewNormal), normalize(-ViewPosition));
    opacity = abs(opacity);
    opacity = 1.0 - pow(opacity, EdgeFalloff);
     
    result *= opacity;

    //Inverted occlusion for more shadow
    float Alpha = 0.5;

    vec4 InvertAO = vec4(vec3(1.0, 1.0, 1.0) - vec3(Occlusion, Occlusion, Occlusion), Alpha);

    return result * InvertAO;

}

////////////////////////////////////////////////////////////////////////////////
/// AMBIENT OCCLUSION
/// Source: Anon, n.d. Learn OpenGL, extensive tutorial resource for learning
/// Modern OpenGL. [online] Learnopengl.com. Available from:
/// https://learnopengl.com/#!Advanced-Lighting/SSAO [Accessed 21 Feb. 2017].
////////////////////////////////////////////////////////////////////////////////
subroutine (RenderType)
vec4 AORender()
{

    return vec4(vec3(Occlusion), 1.0);
}

void main() {
    vec4 color = RenderTypeSelection();
    float alpha = WorldPosition.r == 0.0 ? 0.0 : 1.0;
    color *= vec4(alpha);
    fColor = color ;
}
