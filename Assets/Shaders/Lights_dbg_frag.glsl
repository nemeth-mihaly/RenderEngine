#version 460 core
out vec4 v_FragmentColor;

uniform vec3 u_Color;

void main()
{
    v_FragmentColor = vec4(u_Color, 1.0);
}