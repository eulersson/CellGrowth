#version 450 core

// Ins
in vec3 position;
in vec2 uv;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform mat4 MV;
uniform mat4 MVP;

// Outs
//@brief Frament position on quad.
out vec3 FragPos;
//@brief Directional vector of the light.
out vec3 lightDir;
//@brief Texture coordinates on quad.
out vec2 TexCoord;

uniform int width;
uniform int height;

void main(void)
{
    TexCoord = uv;
    gl_Position = vec4(position, 1.0f);

    FragPos = vec3(ModelMatrix * vec4(position, 1.0f));
    lightDir = normalize(vec3(-1.0, 0.0, 1.0));
}

