#version 450 core

// Uniforms
uniform mat4 ProjectionMatrix;

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

    vec3 newpos = position;
    newpos.x / 200;
    newpos.y / 200;



    gl_Position = ProjectionMatrix * vec4(position, 1.0);
    //gl_PointSize = radius_multiplier * radius / (radius_distance_decay * abs(position.z));
    gl_PointSize = 29.3 *radius;
}
