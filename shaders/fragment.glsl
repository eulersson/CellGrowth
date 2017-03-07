#version 120

// Read docs on gl_PointCoord https://www.opengl.org/sdk/docs/man/html/gl_PointCoord.xhtml
void main() {
    // Defining light
    vec3 lightDir = normalize(vec3(0.0, -1.0, 1.0));

    // Getting the normal using gl_PointCoords, refer to docs
    vec3 N;
    N.xy = gl_PointCoord* 2.0 - vec2(1.0);
    float mag = dot(N.xy, N.xy);
    if (mag > 1.0) discard;   // kill pixels outside circle
    N.z = sqrt(1.0-mag);

    // diffuse coefficient (Kd) and color (diffuseColor)
    float Kd = max(0.0, dot(lightDir, N));
    vec3 diffuseColor = vec3(1.0);

    vec3 color = Kd * diffuseColor;

    // If you want, output N as color to see that the normals are right :)
    gl_FragColor = vec4(color, 1.0);
}
