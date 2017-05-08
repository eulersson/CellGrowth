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
    vec3 colour;
    float Linear;
    float Quadratic;
};

////////////////////////////////////////////////////////////////////////////////
/// Textures
////////////////////////////////////////////////////////////////////////////////
uniform sampler2D tWorldPosition;
uniform sampler2D tViewPosition;
uniform sampler2D tWorldNormal;
uniform sampler2D tViewNormal;
uniform sampler2D tSSAO;
uniform sampler2D tLinks;

////////////////////////////////////////////////////////////////////////////////
/// Uniforms
////////////////////////////////////////////////////////////////////////////////
uniform Material material;
uniform Light light;
uniform bool drawLinks;
uniform mat4 ProjectionMatrix;

////////////////////////////////////////////////////////////////////////////////
/// Globals
////////////////////////////////////////////////////////////////////////////////
vec3 ViewPosition  = texture( tViewPosition, vTexCoords).xyz;
vec3 WorldPosition = texture(tWorldPosition, vTexCoords).xyz;
float Depth = texture(tViewPosition, vTexCoords).z;
vec3 ViewNormal  = normalize(texture( tViewNormal, vTexCoords).xyz);
vec3 WorldNormal = normalize(texture(tWorldNormal, vTexCoords).xyz);
float Occlusion = texture(tSSAO, vTexCoords).r;
float Links = texture(tLinks, vTexCoords).r;

////////////////////////////////////////////////////////////////////////////////
/// Subroutine definition
////////////////////////////////////////////////////////////////////////////////
subroutine vec4 RenderType();
subroutine uniform RenderType RenderTypeSelection;

////////////////////////////////////////////////////////////////////////////////
/// ADS SHADING
/// Source: https://learnopengl.com/#!Lighting/Basic-Lighting
/// Source: https://learnopengl.com/#!Lighting/Materials
////////////////////////////////////////////////////////////////////////////////
subroutine (RenderType)
vec4 ADSRender()
{
    ///! WORK HERE VAL
    vec3 lightDir = normalize(light.position - WorldPosition);
    vec3 viewDir  = normalize(-WorldPosition);

    // Ambient
    vec3 ambient = light.ambient * material.ambient;

    // Diffuse
    vec3 diffuse = max(dot(WorldNormal, lightDir), 0.0) * material.diffuse * light.diffuse;

    // Specular
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(WorldNormal, halfwayDir), 0.0), 8.0);
    vec3 specular = light.specular * spec;

    // Attenuation
    float distance = length(light.position - WorldPosition);
    float attenuation = 1.0 / (1.0 + light.Linear * distance + light.Quadratic * distance * distance);
    diffuse *= attenuation;
    specular *= attenuation;
    
    vec3 lighting = ambient + diffuse + specular;

    lighting *= vec3(Occlusion);

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
    vec4 result = vec4(1.0);
    
    float EdgeFalloff = 2.3;
    float opacity = dot(normalize(ViewNormal), normalize(-ViewPosition));
    opacity = abs(opacity);
    opacity = 1.0 - pow(opacity, EdgeFalloff);
     
    result *= opacity;

    ///! WORK HERE VAL
    return result;
}

////////////////////////////////////////////////////////////////////////////////
/// AMBIENT OCCLUSION
/// Source: https://learnopengl.com/#!Advanced-Lighting/SSAO
////////////////////////////////////////////////////////////////////////////////
subroutine (RenderType)
vec4 AORender()
{
    return vec4(vec3(Occlusion), 1.0);
}

void main() {
    vec4 color = RenderTypeSelection();
    float alpha = length(ViewPosition) == 0.0 ? 0.0 : 1.0;

    // Composite the links on top if needed
    if (drawLinks)
    {
        alpha = clamp(alpha + Links,0,1);
        color = clamp(color + vec4(Links), vec4(0.0), vec4(1.0));
    }

    color *= vec4(alpha);

    fColor = color;
}
