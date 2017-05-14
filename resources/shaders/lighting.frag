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

//struct for the Spot Light
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float Linear;
    float Quadratic;
};

//struct for the Point Light
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
    //Finding the direction of the light.
    vec3 lightDir = normalize(light.position - WorldPosition);
    vec3 fillLightDir = normalize(fillLight.position - WorldPosition);

    //Direction of the camera/eye
    vec3 viewDir  = normalize(-WorldPosition);

    // Calculating ambient
    vec3 ambient = (material.ambient * light.ambient) + (material.ambient * fillLight.ambient);

    // Calculating diffuse
    vec3 diffuse = (max(dot(WorldNormal, lightDir), 0.0) * ((material.diffuse * light.diffuse)) + (max(dot(WorldNormal, fillLightDir), 0.0) * (material.diffuse * fillLight.diffuse)));

    //Finding halfway vector (vector between the view and lightdirection).
    vec3 halfwayDir = normalize(lightDir + viewDir);
    vec3 fillLightHalfVec =  normalize(fillLightDir + viewDir);

    //Calculating specular.
    float spec = pow(max(dot(WorldNormal, halfwayDir), 0.0), 8.0);
    float spec2 = pow(max(dot(WorldNormal, fillLightHalfVec), 0.0), 8.0);
    //Multiply with 0.5 on the fill light, because I want it to be weaker than the main light.
    vec3 specular = (spec * light.specular) + (spec2 * fillLight.specular * 0.5);

    // Attenuation
    float distance = length(light.position - WorldPosition);
    //Light attenuation: linear and quadratic decay.1
    float attenuation = 1.0 / (1.0 + light.Linear * distance + light.Quadratic * distance * distance);
    diffuse *= attenuation;
    specular *= attenuation;
    
    vec3 lighting = (ambient + diffuse + specular);

    lighting = clamp(lighting, vec3(0), vec3(1));

  return vec4(lighting, 1.0);

}

////////////////////////////////////////////////////////////////////////////////
/// X-RAY SHADING
/// Source:
/// Burst, T., 2009. Simple X-Ray Shader. [online] machines don't care.
/// Available from:
/// https://machinesdontcare.wordpress.com/2009/06/09/simple-x-ray-shader
/// [Accessed 14 Mar. 2017].
///
/// Marten, C., 2012. Faking X-Ray rendering. [online] TSG.
/// Available from: http://www.tinysg.de/techGuides/tg2_xray.html
/// [Accessed 14 Mar. 2017].
////////////////////////////////////////////////////////////////////////////////
subroutine (RenderType)
vec4 XRayRender()
{
    //Setting colour to white (we want the edges to be white)
    vec4 result = vec4(1.0, 1.0, 1.0, 1.0);
    
    //Edge falloff decides how blurry the line around the edge will be.
    float EdgeFalloff = 0.7;

    //opacity: if value is close to 0 it will be completely transparent.
    float opacity = dot(normalize(ViewNormal), normalize(-ViewPosition));
    opacity = abs(opacity);
    opacity = 1.0 - pow(opacity, EdgeFalloff);
     
    result *= vec4(opacity, opacity, opacity, opacity);

    //Adding inverted Ambient Occlusion to better be able to see intersection. Also looks better.
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
    //Colour set depending on the subroutine selected.
    vec4 color = RenderTypeSelection();

    //Creating a mask to make background visible.
    float alpha = WorldPosition.r == 0.0 ? 0.0 : 1.0;

    color *= vec4(alpha);
    fColor = color ;
}
