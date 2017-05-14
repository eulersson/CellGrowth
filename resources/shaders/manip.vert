
#version 410 core

in vec3 posAttr;
in vec3 normAttr;

out vec3 vNorm;
out vec3 vPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float lightScale;

void main()
{

    vNorm=normAttr;
    vPos=posAttr;


    float scaleModifier = lightScale; // change value to match resolution.    = (2 * ObjectSizeOnscreenInPixels / ScreenWidthInPixels)
    float w = (projection * view * model * vec4(0,0,0,1)).w;
    w *= scaleModifier;

    gl_Position = projection * view * model * vec4(posAttr*w, 1.0f);

}
