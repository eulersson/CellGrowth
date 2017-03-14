#version 450 core

#define M_PI 3.1415926535897932384626433832795
// Passes for the G-Buffer
layout (location = 0) out vec3 gDepthPass;
layout (location = 1) out vec3 gPositionPass;
layout (location = 2) out vec3 gNormalPass;
layout (location = 3) out vec3 gDiffusePass;
layout (location = 4) out vec3 gSSAONoisePass;

in vec3 vNormal;
in vec3 vViewPosition;

/////////////////////////////////////////////////////////////////////////////////////////////
///                          LINEARIZED DEPTH
///Source: http://www.geeks3d.com/20091216/geexlab-how-to-visualize-the-depth-buffer-in-glsl/
/////////////////////////////////////////////////////////////////////////////////////////////
float linearizedDepth(float zPosValue)
{
    float zDepth = zPosValue;
    float near = 2.5f;
    float far = 9.0f;
    return ((2.0 * near) / (far + near - zDepth*(far - near)));
}

void main(void)
{
    float lDepth = linearizedDepth(vViewPosition.z);

    gPositionPass = vViewPosition;
    gNormalPass = vNormal;
    gDepthPass =  vec3(lDepth, lDepth, lDepth);
    gDiffusePass = vec3(0.95);
    gSSAONoisePass = gSSAONoisePass;
}
