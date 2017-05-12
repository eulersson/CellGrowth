#version 410 core

in vec3 vWorldPosition;
in vec3 vViewPosition;
in vec3 vWorldNormal;

layout (location = 0) out vec4 gWorldPositionPass; // GL_COLOR_ATTACHMENT0
layout (location = 1) out vec4 gViewPositionPass;  // GL_COLOR_ATTACHMENT1
layout (location = 2) out vec4 gWorldNormalPass;   // GL_COLOR_ATTACHMENT2

// We export each pass with alphas for the XRay shader to work. In the actual
// textures no alpha is gonna be stored. It is just for the OpenGL blending
// and OpenGL culling to work properly.
void main() {
    gWorldPositionPass = vec4(vWorldPosition, 1.0);
    gViewPositionPass = vec4(vViewPosition, 1.0);
    gWorldNormalPass = vec4(vWorldNormal, 1.0);
}
