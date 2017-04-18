#version 410 core

uniform sampler2D tDepth;          // Color Attachment 0
uniform sampler2D tPosition;       // Color Attachment 1
uniform sampler2D tNormal;         // Color Attachment 2
uniform sampler2D tDiffuse;        // Color Attachment 3
uniform sampler2D tSSAONoise;      // Color Attachment 4
uniform sampler2D tScreenNormals;  // Color Attachment 5
uniform sampler2D tMask;           // Color Attachment 6
uniform sampler2D tLinks;          // Color Attachment 7

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
uniform bool drawLinks;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 ViewPos;

uniform int width;
uniform int height;



out vec4 FragColor;

//Setting samper2D into vec3. Easier to read code.
vec3 Depth = texture2D(tDepth, TexCoord).rgb;
vec3 Normals = normalize(texture2D(tNormal, TexCoord).rgb);
vec3 Position = texture2D(tPosition, TexCoord).xyz;
vec3 ScreenN = texture2D(tScreenNormals, TexCoord).rgb;
vec4 Diffuse = texture2D(tDiffuse, TexCoord).rgba;


////////////////////////////////////////////////////////////////////////////////
///                      AMBIENT OCCLUSION SHADING
/// Source: https://learnopengl.com/#!Advanced-Lighting/SSAO
////////////////////////////////////////////////////////////////////////////////
vec4 AO()
{
    /*SSAOnoise texture will not tile, finding how
    much it needs to be scaled with.*/
    vec2 noiseScale = vec2(width/4.0, height/4.0);
    float bias = 0.025;
    int kernelSize = 64;
    float radius = 0.5;

    /*
            IMPORTANT NOTE
            Using positions from texture does not work. Issues with world space
            normals. fragmentPos have been replaced with FragPos for now.
    */
    //vec3 fragmentPos = texture2D(tPosition, TexCoord).xyz;


    vec3 randomVec = normalize(texture(tSSAONoise, TexCoord * noiseScale).xyz);
    vec3 tangent = normalize(randomVec - ScreenN * dot(randomVec, ScreenN));
    vec3 bitangent = cross(ScreenN, tangent);
    mat3 TBN = mat3(tangent, bitangent, ScreenN);


    float occlusion = 0.0;

    for(int i = 0; i < kernelSize; ++i)
    {
        vec3 samplePos = TBN * samples[i];
        samplePos = FragPos + samplePos * radius;

        vec4 offset = vec4(samplePos, 1.0);
        offset = ProjectionMatrix * offset;
        offset.xy /= offset.w;
        offset.xy = offset.xy * 0.5 + 0.5;

        float sampleDepth = texture2D(tPosition, offset.xy).z;
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(FragPos.z - sampleDepth));
        occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;
    }

    occlusion = 1.0 - (occlusion / kernelSize);

    //Blur
    vec2 texelSize = 1.0 / vec2(textureSize(tMask, 0));
    float result = 0.0;
    for (int x = -2; x < 2; ++x)
    {
        for (int y = -2; y < 2; ++y)
        {
            vec2 offset = vec2(float(x), float (y)) * texelSize;
            result =+ texture2D(tMask, TexCoord + offset).r;
        }
    }

    //Applying
    vec3 ambient = vec3(0.3 * Diffuse.rgb * occlusion);
    vec3 lighting = ambient;
    vec3 viewDir = normalize( -FragPos );

    vec3 lightDir = normalize(light.position - FragPos);
    vec3 diffuse = max(dot(ScreenN, lightDir), 0.0) * Diffuse.rgb * light.colour;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(ScreenN, halfwayDir), 0.0), 8.0);
    vec3 specular = light.colour * spec;

    float dist = length(light.position - FragPos);
    float attenuation = 1.0/(1.0 + light.Linear * dist + light.Quadratic * dist * dist);
    diffuse *= attenuation;
    specular *= attenuation;
    lighting += diffuse + specular;

    return vec4(lighting, 1.0);

}

//Inverted AO for enhancing the xRay.
vec4 invrtAO()
{
    return vec4(1.0, 1.0, 1.0, 1.0) - AO();
}


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
    //float Grey = dot(texture(tPosition, TexCoord).rgb, vec3(texture(tPosition, TexCoord).r, texture(tPosition, TexCoord).g,texture(tPosition, TexCoord).b));

   // vec3 Depth = texture(depth, TexCoord).rgb;

    //Ambient
    vec3 Ambient = light.ambient * material.ambient;

    //Diffuse
    vec3 sampledNormal = texture(tScreenNormals, TexCoord).rgb;

    vec3 norm = sampledNormal;
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 Diffuse = light.diffuse * (diff * material.diffuse);

    //Specular Light
    vec3 viewDir =  normalize(ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 Specular = light.specular * (spec * material.specular);

    return vec4((Ambient + Diffuse + Specular) * Depth * light.colour, 1.0f);
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
    float Grey = dot(texture(tScreenNormals, TexCoord).rgb, vec3(texture(tScreenNormals, TexCoord).r, texture(tScreenNormals, TexCoord).g,texture(tScreenNormals, TexCoord).b));

    //Transparent Opacity
    // float Grey = dot(texture(depth, TexCoord).rgb, vec3(texture(depth, TexCoord).r, texture(depth, TexCoord).g,texture(depth, TexCoord).b));

    vec3 XRay = vec3(Grey, Grey, Grey);

    return vec4(XRay * light.colour, 1.0f) * invrtAO();;
}


subroutine (ShadingPass)
vec4 AORender()
{
    return AO();

}

void main(void)
{
    vec4 mask = texture(tMask, TexCoord).rgba;

    vec4 linksCol = texture(tLinks, TexCoord).rgba;
    vec4 color = ShaderPassSelection();

    if (drawLinks) {
        color = vec4(max(linksCol.rgb, color.rgb), max(Diffuse.a, linksCol.a));
    }

    FragColor = color;
}
