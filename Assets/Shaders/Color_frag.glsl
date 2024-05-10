#version 460 core
out vec4 v_FragmentColor;

void main()
{
    vec3 purple = vec3(1.0, 0.0, 1.0);
    v_FragmentColor = vec4(purple, 1.0);
}