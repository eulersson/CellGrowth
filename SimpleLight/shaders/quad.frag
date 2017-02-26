#version 450 core

// Subroutine declaration and uniform handle
subroutine vec3 RenderPassType();
subroutine uniform RenderPassType RenderPassSelection;

// Texture passes coming from the FBO
uniform sampler2D gpass0Tex;
uniform sampler2D gpass1Tex;

// Data coming from vertex shader
in vec2 texCoord;

// Color output
out vec4 FragColor;

// Subroutine definition: Normal Shading
subroutine (RenderPassType)
vec3 normalShading()
{
    vec3 pass0 = texture2D(gpass0Tex, texCoord).rgb;
    return pass0;
}

// Subroutine definition: Position Shading
subroutine (RenderPassType)
vec3 positionShading()
{
    vec3 pass1 = texture2D(gpass1Tex, texCoord).rgb;
    return pass1;
}

void main(void)
{
    vec3 color = RenderPassSelection();
    FragColor = vec4(color, 1.0);
}
