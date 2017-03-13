#version 450 core

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

void main(void)
{
    vNormal = normalize(position);
    vViewPosition = vec4(ViewMatrix * ModelMatrix * vec4(instances.w * position + instances.xyz, 1.0)).xyz;
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(instances.w * position + instances.xyz, 1.0);
}
