#version 410 core

// Ins
in vec3 position;
in vec2 uv;
in vec3 viewPos;
in vec3 lightPos;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform mat4 MV;
uniform mat4 MVP;

// Outs
//@brief Frament position on quad.
out vec3 FragPos;
//@brief Directional vector of the light.
out vec3 LightPos;
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
   // LightPos = normalize(lightPos);
    LightPos = lightPos;
    ViewPos = viewPos;
}

