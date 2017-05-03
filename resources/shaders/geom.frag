#version 410 core

in vec3 vPosition;
in vec3 vNormal;
in vec3 vWorldNormal;

layout (location = 0) out vec3 gPositionPass;      // GL_COLOR_ATTACHMENT0
layout (location = 1) out vec3 gNormalPass;        // GL_COLOR_ATTACHMENT1
layout (location = 2) out vec3 gWorldNormalPass;   // GL_COLOR_ATTACHMENT2
layout (location = 3) out vec3 gWorldPositionPass; // GL_COLOR_ATTACHMENT3

void main() {
  gPositionPass = vPosition;
  gNormalPass = normalize(vNormal);
  gWorldNormalPass = normalize(vWorldNormal);
}