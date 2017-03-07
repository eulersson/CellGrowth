#version 450 core

// Uniforms
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

// Ins
in vec3 position;
in float radius;

// Outs
out vec3 worldPosition;
out float partRadius;

// The bitmap that is drawn it has dimensions (pixels): radius_multiplier * radius
const float radius_multiplier = 40.0;

// The further the particle is the smaller the bitmap is drawn
const float radius_distance_decay = 0.2;

void main(void)
{    
    worldPosition = position;
    partRadius = radius;

    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position, 1.0);
    gl_PointSize = radius_multiplier * radius / (radius_distance_decay * abs(position.z));
}
