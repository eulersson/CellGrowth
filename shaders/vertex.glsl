attribute highp vec3 posAttr;
void main()
{
    gl_Position = vec4(posAttr, 1.0);
}
