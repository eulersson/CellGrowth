//attribute highp vec3 posAttr;
#version 330 core
attribute highp vec3 posAttr;
attribute highp vec3 normAttr;
layout (location = 0) in vec3 position;
layout (location = 2) in vec2 texCoord;

out vec2 TexCoord;
out vec3 vNorm;
out vec3 vPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{

    vNorm=normAttr;
    vPos=posAttr;

    //gl_Position = vec4(posAttr, 1.0);
    //gl_Position = model * view * projection * vec4(posAttr, 1.0f);
    gl_Position = projection * view * model * vec4(posAttr, 1.0f);
    TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);

}
