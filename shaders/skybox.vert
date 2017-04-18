#version 410 core

in vec3 pos;
uniform mat4 Projection;
uniform mat4 View;
out vec3 TexCoords;

void main() {
  TexCoords = pos;
  gl_Position = Projection * View * vec4(20.0 * pos, 1.0);
}
