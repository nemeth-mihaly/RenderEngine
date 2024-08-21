#version 460 core

in layout(location = 0) vec3 aPos;
in layout(location = 1) vec3 aColor;

out DATA
{
    vec3    Pos;
    vec3    Color;
} data;

uniform mat4 uModel, uView, uProj;

void main()
{
    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);

    data.Pos = aPos;
    data.Color = aColor;
}