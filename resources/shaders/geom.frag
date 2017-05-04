#version 410 core

in vec3 vWorldPosition;
in vec3 vViewPosition;
in vec3 vWorldNormal;
in vec3 vViewNormal;

layout (location = 0) out vec3 gWorldPositionPass; // GL_COLOR_ATTACHMENT0
layout (location = 1) out vec3 gViewPositionPass;  // GL_COLOR_ATTACHMENT1
layout (location = 2) out vec3 gWorldNormalPass;   // GL_COLOR_ATTACHMENT2
layout (location = 3) out vec3 gViewNormalPass;    // GL_COLOR_ATTACHMENT3

void main() {
    gWorldPositionPass = vWorldPosition;
    gViewPositionPass = vViewPosition;
    gWorldNormalPass = vWorldNormal;
    gViewNormalPass = vViewNormal;
}