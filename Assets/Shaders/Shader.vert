#version 460 core

in layout(location = 0) vec3 aPos;
in layout(location = 1) vec3 aColor;

out DATA
{
    vec3    Pos;
    vec3    Color;
} data;

void main()
{
    gl_Position = vec4(aPos, 1.0);

    data.Pos = aPos;
    data.Color = aColor;
}