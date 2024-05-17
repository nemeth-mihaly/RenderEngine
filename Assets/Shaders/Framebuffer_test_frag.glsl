#version 460 core
out vec4 v_FragmentColor;

in vec2 v_Uv;

uniform sampler2D u_Texture;

void main()
{
    v_FragmentColor = texture(u_Texture, v_Uv);
    //v_FragmentColor = vec4(vec3(1.0 - texture(u_Texture, v_Uv)), 1.0);
}