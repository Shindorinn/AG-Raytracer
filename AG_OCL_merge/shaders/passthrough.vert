#version 330 compatibility

out vData
{
    vec3 normal;
    vec4 color;
}vertex;

void main()
{
    vertex.normal = normalize(gl_NormalMatrix * gl_Normal);
    vertex.color = gl_Color;
    gl_Position = ftransform();
}