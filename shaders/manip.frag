
#version 410 core

in vec4 vColour;
in vec3 vNorm;
in vec3 vPos;
out vec4 fColour;

uniform vec3 baseColour; // Colour of arrow
uniform vec3 renderColour; // Highlight
uniform bool backRender=false;

const vec3 AMBIENT_LIGHT=vec3(0.3,0.3,0.3);

void main() {
    // Fragment shader is being applied to all points of fragment

    vec3 lightPos = vec3(10,2,0);

//    // Defining light
    vec3 lightDir = normalize(vec3(0.0, -1.0, 1.0));
    lightDir=normalize(lightPos-vPos);


    vec3 n = vNorm;
//    // diffuse coefficient (Kd) and color (diffuseColor)
    float Kd = max(0.0, dot(lightDir, n));
    vec3 diffuseColor = vec3(.2);

    // Currently setting fragment to plain colour
    vec3 finalColour = Kd * diffuseColor;
    finalColour+=renderColour+baseColour;


    // Must be last part of shading
    if(backRender)
    {
        finalColour=baseColour/255;
    }



    // If you want, output N as color to see that the normals are right :)
    //gl_FragColor = vec4(color, 1.0);
    fColour = vec4(finalColour, 1.0);

}
