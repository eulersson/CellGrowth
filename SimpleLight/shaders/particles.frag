#version 450 core

#define M_PI 3.1415926535897932384626433832795

// Passes for the G-Buffer
layout (location = 0) out vec3 gNormalPass;
layout (location = 1) out vec3 gPositionPass;

// Data coming from vertex shader
in vec3 worldPosition;
in vec3 partRadius;

void main(void)
{
    vec3 N;
    N.xy = gl_PointCoord* 2.0 - vec2(1.0);
    float mag = dot(N.xy, N.xy);
    if (mag > 1.0) discard;
    N.z = sqrt(1.0-mag);

    gNormalPass   = N;
    gPositionPass = worldPosition;
}
