
#version 450 core
in vec3 posAttr;
in vec3 normAttr;

out vec3 vNorm;
out vec3 vPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{

    vNorm=normAttr;
    vPos=posAttr;

    gl_Position = projection * view * model * vec4(posAttr, 1.0f);

}
