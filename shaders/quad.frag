#version 450 core

uniform sampler2D depth;         // Color Attachment 0
uniform sampler2D positionTex;   // Color Attachment 1
uniform sampler2D normal;        // Color Attachment 2
uniform sampler2D diffuse;       // Color Attachment 3
uniform sampler2D ssaoNoiseTex;  // Color Attachment 4
uniform sampler2D ScreenNormals; //Colour Attachment 5

uniform vec3 samples[64];


uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform mat4 MV;
uniform mat4 MVP;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    float attenuation;
};

uniform Material material;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 colour;

    float Linear;
    float Quadratic;
    float Radius;

};

uniform Light light;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 LightPos;
in vec3 ViewPos;

uniform int width;
uniform int height;


float bias = 0.025;
int kernelSize = 64;
float radius = 0.5;
/*SSAOnoise texture will not tile, finding how
much it needs to be scaled with.*/
const vec2 noiseScale = vec2(width/4.0, height/4.0);


out vec4 FragColor;


subroutine vec4 ShadingPass();
subroutine uniform ShadingPass ShaderPassSelection;



////////////////////////////////////////////////////////////////////////////////
///                         ADS SHADING
/// Source: https://learnopengl.com/#!Lighting/Basic-Lighting
/// Source: https://learnopengl.com/#!Lighting/Materials
////////////////////////////////////////////////////////////////////////////////
subroutine (ShadingPass)
vec4 ADSRender()
{
    //Particle positions in greyscale.
    //float Grey = dot(texture2D(positionTex, TexCoord).rgb, vec3(texture2D(positionTex, TexCoord).r, texture2D(positionTex, TexCoord).g,texture2D(positionTex, TexCoord).b));


    vec3 depth = texture2D(depth, TexCoord).rgb;


    //Ambient
    vec3 ambient = light.ambient * material.ambient;

    //Diffuse
    vec3 sampledNormal = texture2D(ScreenNormals, TexCoord).rgb;

    vec3 norm = sampledNormal;
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 ddiffuse = light.diffuse * (diff * material.diffuse);

    //Specular Light
    vec3 viewDir =  normalize(ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    return vec4((ambient + ddiffuse + specular) * depth * light.colour, 1.0f);

}



////////////////////////////////////////////////////////////////////////////////
///                             X-RAY SHADING
/// Source: https://machinesdontcare.wordpress.com/2009/06/09/simple-x-ray-shader
/// Source: http://www.tinysg.de/techGuides/tg2_xray.html
////////////////////////////////////////////////////////////////////////////////
subroutine (ShadingPass)
vec4 xRayRender()
{
    // Translucent Opacity
    float Grey = dot(texture2D(ScreenNormals, TexCoord).rgb, vec3(texture2D(ScreenNormals, TexCoord).r, texture2D(ScreenNormals, TexCoord).g,texture2D(ScreenNormals, TexCoord).b));

    //Transparent Opacity
    // float Grey = dot(texture2D(depth, TexCoord).rgb, vec3(texture2D(depth, TexCoord).r, texture2D(depth, TexCoord).g,texture2D(depth, TexCoord).b));

    vec3 XRay = vec3(Grey, Grey, Grey);

    return vec4(XRay * light.colour, 1.0f);
}



////////////////////////////////////////////////////////////////////////////////
///                      AMBIENT OCCLUSION SHADING
/// Source: https://learnopengl.com/#!Advanced-Lighting/SSAO
////////////////////////////////////////////////////////////////////////////////
subroutine (ShadingPass)
vec4 AORender()
{
    vec3 fragPos = texture(positionTex, TexCoord).xyz;
    vec3 normal = normalize(texture(ScreenNormals, TexCoord).rgb);
    vec3 randomVec = normalize(texture(ssaoNoiseTex, TexCoord * noiseScale).xyz);

    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);


    float occlusion = 0.0;

    for(int i = 0; i < kernelSize; ++i)
    {
        vec3 samplePos = TBN * samples[i];
        samplePos = fragPos + samplePos * radius;

        vec4 offset = vec4(samplePos, 1.0);
        offset = ProjectionMatrix * offset;
        offset.xy /= offset.w;
        offset.xy = offset.xy * 0.5 + 0.5;

        float sampleDepth = texture(positionTex, offset.xy).z;
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;
    }

    occlusion = 1.0 - (occlusion / kernelSize);

    return vec4(vec3(occlusion), 1.0f);

}




void main(void)
{
    vec4 color = ShaderPassSelection();
    FragColor = color;
}
