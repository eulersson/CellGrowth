#version 410 core

in vec3 position;
in vec2 uv;

out vec2 vTexCoords;

void main() {
  vTexCoords = uv;
  gl_Position = vec4(position, 1.0);
}