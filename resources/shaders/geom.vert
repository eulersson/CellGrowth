#version 410 core

// Uniforms
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

// Ins
in vec3 position;
in vec4 instances; // Each instance will have x,y,z and w (radius)

// Outs
out vec3 vWorldPosition;
out vec3 vViewPosition;
out vec3 vWorldNormal;
out vec3 vViewNormal;

void main(void)
{
    vWorldPosition = instances.w * position + instances.xyz;
    vViewPosition = vec4(ViewMatrix * ModelMatrix * vec4(vWorldPosition, 1.0)).xyz;

    mat3 normalMatrix = transpose(inverse(mat3(ViewMatrix * ModelMatrix)));
    vWorldNormal = normalize(position);
    vViewNormal = normalize(normalMatrix * normalize(position));

    gl_Position = ProjectionMatrix * vec4(vViewPosition, 1.0);
}
