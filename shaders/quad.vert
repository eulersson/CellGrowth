#version 450 core

// Ins
in vec3 position;
in vec2 uv;

// Outs
out vec2 texCoord;

void main(void)
{
    texCoord = uv;
    gl_Position = vec4(position, 1.0);
}
