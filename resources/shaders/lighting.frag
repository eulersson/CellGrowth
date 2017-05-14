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
    vec3 lightDir = normalize(light.position - WorldPosition);
    vec3 fillLightDir = normalize(fillLight.position - WorldPosition);
    vec3 viewDir  = normalize(-WorldPosition);

    // Ambient
    vec3 ambient = (material.ambient * light.ambient) + (material.ambient * fillLight.ambient);

    // Diffuse
    vec3 diffuse = (max(dot(WorldNormal, lightDir), 0.0) * ((material.diffuse * light.diffuse)) + (max(dot(WorldNormal, fillLightDir), 0.0) * (material.diffuse * fillLight.diffuse)));

    // Specular
    vec3 halfwayDir = normalize(lightDir + viewDir);
    vec3 fillLightHalfVec =  normalize(fillLightDir + viewDir);
    float spec = pow(max(dot(WorldNormal, halfwayDir), 0.0), 8.0);
    float spec2 = pow(max(dot(WorldNormal, fillLightHalfVec), 0.0), 8.0);
    vec3 specular = (spec * light.specular) + (spec2 * fillLight.specular * 0.5);

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
    //Setting colour to white (we want the edges to be white)
    vec4 result = vec4(1.0, 1.0, 1.0, 1.0);
    
    float EdgeFalloff = 0.7;
    float opacity = dot(normalize(ViewNormal), normalize(-ViewPosition));
    opacity = abs(opacity);
    opacity = 1.0 - pow(opacity, EdgeFalloff);
     
    result *= opacity;

    //Inverted occlusion for more shadow
    vec4 InvertAO = vec4(vec3(1.0, 1.0, 1.0) - vec3(Occlusion, Occlusion, Occlusion), 0.1);

    return result + InvertAO;

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


///////////////////////////////////////////////////////////////////////////////
/// NEW ORDER ARTSTYLE
/// Inspired by Peter Saville and New Order: Technique (Facotry, 1989)
//////////////////////////////////////////////////////////////////////////////
subroutine (RenderType)
vec4 NewOrderRender()
{
    //Starting with normal view colours.
    vec3 newOrder = ViewNormal;
    //Increasing blue and red values, and less green.
    newOrder.z += newOrder.y * 2.0;

    //Adding a little bit of green back.
    vec3 green = vec3(0.0, 0.5, 0.0);

    return vec4(newOrder + green, 1.0);
}

void main() {
    vec4 color = RenderTypeSelection();
    float alpha = WorldPosition.r == 0.0 ? 0.0 : 1.0;
    color *= vec4(alpha);
    fColor = color ;
}
