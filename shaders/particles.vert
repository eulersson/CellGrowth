#version 450 core

// Uniforms
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

// Ins
in vec3 position;
in vec4 instances;

// Outs
out vec4 worldPosition;
out float partRadius;
out vec3 vNormal;


void main(void)
{
    vNormal = normalize(position);

    worldPosition = ModelMatrix * vec4(instances.w * position + instances.xyz, 1.0);
    partRadius = instances.w;

    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(instances.w * position + instances.xyz, 1.0);
}
