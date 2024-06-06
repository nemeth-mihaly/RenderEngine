#version 460 core
out vec4 v_FragmentColor;

in vec2 v_Uv;
in float g_height;

uniform sampler2D u_Texture;

void main()
{
    float h = (g_height + 16) / 32;
    //v_FragmentColor = texture(u_Texture, v_Uv);
    v_FragmentColor = vec4(h, h, h, 1.0);
}