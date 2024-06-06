#version 460 core

in layout(location = 0) vec3 a_Pos;
in layout(location = 1) vec3 a_Normal;
in layout(location = 2) vec2 a_Uv;

layout (std140) uniform Matrices
{
    mat4 Projection;
    mat4 View;
};
 
out vec2 v_Uv;
out float g_height;

void main()
{
    gl_Position = Projection * View * vec4(a_Pos, 1.0);
    v_Uv = a_Uv;
    g_height = a_Pos.y;
}