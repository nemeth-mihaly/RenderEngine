#version 460 core

in layout(location = 0) vec3 a_Pos;
in layout(location = 1) vec3 a_Normal;
in layout(location = 2) vec2 a_Uv;

out vec2 v_Uv;

void main()
{
    vec4 pos = vec4(a_Pos.x, a_Pos.y, 0.0, 1.0);
    gl_Position = pos;
    v_Uv = a_Uv;
}