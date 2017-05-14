
#version 410 core

// Read docs on gl_PointCoord https://www.opengl.org/sdk/docs/man/html/gl_PointCoord.xhtml

out vec4 fColour;

uniform vec3 renderColour;


void main() {
    // Fragment shader is being applied to all points of fragment

//    // Defining light
    vec3 lightDir = normalize(vec3(0.0, 0.0, 1.0));

    vec3 diffuseColor;

    // Getting the normal using gl_PointCoords, refer to docs
    vec3 N;
    N.xy = gl_PointCoord* 2.0 - vec2(1.0);

    float mag = dot(N.xy, N.xy);
    if (mag > 1.0) discard;   // kill pixels outside circle
    if (mag>0.6) diffuseColor=vec3(0, 0, 0);
    else diffuseColor = vec3(2, 1.6, .4);
    N.z = sqrt(1.0-mag);

    float Kd = max(0.0, dot(lightDir, N));

    Kd=max(0.5 , 1-Kd);
    vec3 finalColour = Kd * diffuseColor;
    fColour = vec4(finalColour, 1.0);

}
