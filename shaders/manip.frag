
#version 450 core
in vec4 vColour;
in vec3 vNorm;
in vec3 vPos;
out vec4 fColour;

uniform vec3 baseColour; // Colour of arrow
uniform vec3 renderColour; // Highlight
uniform bool backRender=false;
uniform bool flatRender=false;

const vec3 AMBIENT_LIGHT=vec3(0.3,0.3,0.3);

void main() {

  vec3 finalColour=vec3(1.0,1.0,1.0);
  float alpha=1.0f;
  if(!flatRender)
  {
    vec3 lightPos = vec3(10,2,0);

    // Defining light
    vec3 lightDir = normalize(vec3(0.0, -1.0, 1.0));
    lightDir=normalize(lightPos-vPos);

    vec3 n = vNorm;
    // diffuse coefficient (Kd) and color (diffuseColor)
    float Kd = max(0.0, dot(lightDir, n));
    vec3 diffuseColor = vec3(.2);

    // Currently setting fragment to plain colour
    finalColour = Kd * diffuseColor;
    finalColour+=renderColour+baseColour;
  }

  else
  {
    finalColour=vec3(0.6);
    alpha=0.2f;
  }

  // Must be last part of shading
  if(backRender)
  {
      finalColour=baseColour/255;
  }

  fColour = vec4(finalColour, 1.0);

}
