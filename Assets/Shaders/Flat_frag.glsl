#version 460 core
out vec4 v_FragmentColor;

in vec2 v_Texcoord;

uniform sampler2D u_Texture;

void main()
{
    //v_FragmentColor = vec4(v_Color, 1.0);
    v_FragmentColor = texture(u_Texture, v_Texcoord);
}