#version 460 core

in layout(location = 0) vec3 aPos;
in layout(location = 1) vec3 aColor;

layout (std140, binding = 0) uniform Matrices
{
    mat4 View;
    mat4 Proj;
};

out DATA
{
    vec3    Pos;
    vec3    Color;
} data;

uniform mat4 uModel;

void main()
{
    gl_Position = Proj * View * uModel * vec4(aPos, 1.0);

    data.Pos = aPos;
    data.Color = aColor;
}