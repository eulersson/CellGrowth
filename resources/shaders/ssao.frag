#version 410 core

out float fColor;

in vec2 vTexCoords;

uniform sampler2D tViewPosition;
uniform sampler2D tViewNormal;
uniform sampler2D tTexNoise;

uniform vec3 samples[64];
uniform mat4 ProjectionMatrix;
uniform int width;
uniform int height;

uniform float Radius;
uniform float Bias;
int KernelSize = 64;

void main() {
    // Calculate noise scale to repeat
    vec2 noiseScale = vec2(width/4.0, height/4.0); 

    // Get the needed inputs
    vec3 position = texture(tViewPosition, vTexCoords).rgb;
    vec3 normal = texture(tViewNormal, vTexCoords).rgb;
    vec3 randomVector = normalize(texture(tTexNoise, vTexCoords * noiseScale).xyz);

    // TBN change-of-basis matrix: Change from tangent-space to view-space
    vec3 tangent = normalize(randomVector - normal * dot(randomVector, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);

    // Iterate over sample kernel and calculate occlusion factor
    float occlusion = 0.0;
    for (int i = 0; i < KernelSize; ++i)
    {
        // Get sample position
        vec3 _sample = TBN * samples[i];
        _sample = position + _sample * Radius;

        // Project sample position (to sample texture) (to get position on screen/texture)
        vec4 offset = vec4(_sample, 1.0);
        offset = ProjectionMatrix * offset;                  // from view to clip-space
        offset.xyz /= offset.w;               // perspective divide
        offset.xyz = offset.xyz * 0.5 + 0.5;  // transform to range 0.0 - 1.0

        // Get sample depth
        float sampleDepth = texture(tViewPosition, offset.xy).z;

        // Range check and accumulate
        float rangeCheck = smoothstep(0.0, 1.0, Radius / abs(position.z - sampleDepth));
        occlusion += (sampleDepth >= _sample.z + Bias ? 1.0 : 0.0) * rangeCheck;
    }

    occlusion = 1.0 - (occlusion / KernelSize);
    fColor = occlusion;
}
