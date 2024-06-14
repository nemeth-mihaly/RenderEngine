#version 460 core

in layout(location = 0) vec3 v;

layout (std140) uniform Matrices
{
    mat4 Projection;
    mat4 View;
};

void main()
{
    gl_Position = Projection * View * mat4(1) * vec4(v, 1);
}