void main() {
    vec3 lightDir = normalize(vec3(0.0, -1.0, 1.0));

    vec3 N;
    N.xy = gl_PointCoord* 2.0 - vec2(1.0);
    float mag = dot(N.xy, N.xy);
    if (mag > 1.0) discard;   // kill pixels outside circle
    N.z = sqrt(1.0-mag);

    float diffuse = max(0.0, dot(lightDir, N));

    vec3 color = diffuse * vec3(1.0);

    gl_FragColor = vec4(color, 1.0);


}
