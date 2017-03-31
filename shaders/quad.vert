#version 410 core

// Ins
in vec3 position;
in vec2 uv;
in vec3 viewPos;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

// Outs
//@brief Frament position on quad.
out vec3 FragPos;
//@brief Texture coordinates on quad.
out vec2 TexCoord;

out vec3 ViewPos;

uniform int width;
uniform int height;

void main(void)
{
    TexCoord = uv;
    gl_Position = vec4(position, 1.0f);

    FragPos = vec3(ModelMatrix * vec4(position, 1.0f));

    ViewPos = viewPos;
}

