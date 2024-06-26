#version 460 core

in layout(location = 0) vec3 a_Pos;
in layout(location = 1) vec3 a_Normal;
in layout(location = 2) vec2 a_Uv;

layout (std140) uniform Matrices
{
    mat4 Projection;
    mat4 View;
};

uniform mat4 u_WorldView;
uniform mat4 u_WorldProjection;

out vec2 v_Uv;

void main()
{
    //vec4 pos = Projection * mat4(mat3(View)) * vec4(a_Pos, 1.0);
    vec4 pos = u_WorldProjection * u_WorldView * vec4(a_Pos, 1.0);
    gl_Position = pos.xyww;
    v_Uv = a_Uv;
}