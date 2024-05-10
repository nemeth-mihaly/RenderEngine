#version 460 core

in layout(location = 0) vec3 a_Pos;
in layout(location = 1) vec3 a_Normal;
in layout(location = 2) vec2 a_Uv;

uniform mat4 u_WorldViewProjection;
uniform mat4 u_World;

out vec2 v_Uv;

void main()
{
    gl_Position = u_WorldViewProjection * u_World * vec4(a_Pos, 1.0);
    v_Uv = a_Uv;
}