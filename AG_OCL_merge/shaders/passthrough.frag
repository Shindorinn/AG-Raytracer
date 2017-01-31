#version 330 compatibility

in fData
{
    vec3 normal;
    vec4 color;
}frag;

void main()
{
    gl_FragColor = frag.color;
}