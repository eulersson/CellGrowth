attribute vec3 position;
attribute vec2 uv;

varying vec2 texCoord;

void main(void)
{
    texCoord = uv;
    gl_Position = vec4(position, 1.0);
}
