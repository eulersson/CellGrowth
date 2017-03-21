#version 450 core

#define M_PI 3.1415926535897932384626433832795
// Passes for the G-Buffer
layout (location = 0) out vec3 gDepthPass;              // Color Attachment 0
layout (location = 1) out vec3 gPositionPass;           // Color Attachment 1
layout (location = 2) out vec3 gNormalPass;             // Color Attachment 2
layout (location = 3) out vec3 gDiffusePass;            // Color Attachment 3
layout (location = 4) out vec3 gSSAONoisePass;          // Color Attachment 4
layout (location = 5) out vec3 gScreenNormalPass;       // Color Attachment 5


in vec3 vNormal;
in vec3 vViewPosition;
in vec3 vScreenSpaceNormals;

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

    gDepthPass =  vec3(lDepth);
    gPositionPass = vViewPosition;
    gNormalPass = vNormal;
    gDiffusePass = vec3(0.95);
    gSSAONoisePass = gSSAONoisePass;
    gScreenNormalPass = vScreenSpaceNormals; //World Space Normal Pass.
}
