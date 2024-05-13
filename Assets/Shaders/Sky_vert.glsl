#version 460 core

in layout(location = 0) vec3 a_Pos;
in layout(location = 1) vec3 a_Normal;
in layout(location = 2) vec2 a_Uv;

uniform mat4 u_WorldView;
uniform mat4 u_WorldProjection;

out vec3 v_Uv;

void main()
{
    gl_Position = u_WorldProjection * u_WorldView * vec4(a_Pos, 1.0);
    v_Uv = a_Pos;
}