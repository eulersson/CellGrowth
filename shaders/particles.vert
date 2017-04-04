#version 410 core

// Uniforms
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

// Ins
in vec3 position;
in vec4 instances; // Each instance will have x,y,z and w (radius)

// Outs
out vec3 vNormal;
out vec3 vViewPosition;
out vec3 vScreenSpaceNormals;

void main(void)
{
    vNormal = normalize(position);
    vViewPosition = position;
    //vViewPosition = vec4(ViewMatrix * ModelMatrix * vec4(instances.w * position + instances.xyz, 1.0)).xyz;

    mat3 normalMatrix = transpose(inverse(mat3(ViewMatrix * ModelMatrix)));
    vScreenSpaceNormals = normalMatrix * vNormal;

    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(instances.w * position + instances.xyz, 1.0);
}
