#version 410 core

in vec3 TexCoords;
out vec4 fColor;

void main() {
    fColor = vec4(TexCoords,1.0);
}
