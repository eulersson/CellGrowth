varying vec2 texCoord;

void main(void)
{
    gl_FragColor = vec4(texCoord.x, texCoord.y, 0.0, 1.0);
}
