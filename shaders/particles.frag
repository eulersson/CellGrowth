#version 450 core

#define M_PI 3.1415926535897932384626433832795

// Passes for the G-Buffer
layout (location = 0) out vec3 gNormalPass;
layout (location = 1) out vec3 gPositionPass;

// Data coming from vertex shader
in vec3 vNormal;
in vec3 vViewPosition;

void main(void)
{
    gNormalPass   = vNormal;
    gPositionPass = vViewPosition.xyz;
}
