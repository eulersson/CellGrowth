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

/*SSAOnoise texture will not tile, finding how
much it needs to be scaled with.*/
const vec2 noiseScale = vec2(720/4.0, 720/4.0);


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
    vec3 positions = texture2D(positionTex, TexCoord).xyz;
    vec3 normals = normalize(texture2D(ScreenNormals, TexCoord).rgb);

    //Random rotationfor each fragment.
    vec3 randomVec = texture(ssaoNoiseTex, TexCoord * noiseScale).xyz;

    //GrammSchmidt process
    vec3 tangent = normalize(randomVec - normals * dot(randomVec, normals));
    vec3 bitangent = cross(normals, tangent);
    mat3 TBN = mat3(tangent, bitangent, normals);

    //Iterating over Kernel samples taking them from tangent to view-space.

    float bias = 0.025;
    int KernelSize = 64;
    float radius = 0.5;
    vec3 samplePos;
    float occlusion = 0.0f;


    for(int i = 0; i < KernelSize; i++)
   {
        samplePos = TBN * samples[i];
        samplePos = samplePos * radius + positions;
    };

    vec4 offset = vec4(samplePos, 1.0);
    offset = ProjectionMatrix * offset;
    offset.xy /= offset.w;
    offset.xy = offset.xy * 0.5 + 0.5;


    float sampleDepth =  texture2D(positionTex, offset.xy).r;

    occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0);

    float rangeCheck = smoothstep(0.0, 1.0, radius / abs(positions.z - sampleDepth));
    occlusion += (sampleDepth <= samplePos.z ? 1.0 : 0.0) * rangeCheck;


    occlusion = 1.0 - (occlusion / KernelSize);

    //creating lighting pass
    vec3 lightpositions = texture2D(positionTex, offset.xy).rgb;

    vec3 Diffuse = texture2D(diffuse, TexCoord).rgb;
    float AO = Diffuse.r;




    // Calculate light
    vec3 ambient = vec3(0.3 * Diffuse * AO);
    vec3 lighting  = ambient;
    vec3 viewDir  = normalize(-positions); // Viewpos is (0.0.0)

    // Diffuse
    vec3 lightDir = normalize(light.position - positions);
    vec3 diffuse = max(dot(normals, lightDir), 0.0) * Diffuse * light.colour;

    // Specular
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normals, halfwayDir), 0.0), 8.0);
    vec3 specular = light.colour * spec;

    // Attenuation
    float distance = length(light.position - positions);
    float attenuation = 1.0 / (1.0 + light.Linear * distance + light.Quadratic * distance * distance);
    diffuse *= attenuation;
    specular *= attenuation;
    lighting += diffuse + specular;


    return vec4(vec3(occlusion), 1.0);

   // return vec4(texture2D(ScreenNormals, TexCoord).rgb, 1.0f);
   // return vec4(texture2D(normal, TexCoord).rgb, 1.0f);

}




void main(void)
{
    vec4 color = ShaderPassSelection();
    FragColor = color;
}
