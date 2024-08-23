#version 460 core

out vec4 fragColor;

in DATA
{
    vec3    Pos;
    vec3    Color;
} data;

void main()
{
    fragColor = vec4(data.Color, 1.0);
}