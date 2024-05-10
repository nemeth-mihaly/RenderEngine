#version 460 core
out vec4 v_FragmentColor;

in vec2 v_Texcoord;

uniform sampler2D u_Texture;
uniform int u_bHasTexture;

void main()
{
    if (u_bHasTexture == 1)
    {
        v_FragmentColor = texture(u_Texture, v_Texcoord);
    }
    else
    {
        vec3 purple = vec3(1.0, 0.0, 1.0);
        v_FragmentColor = vec4(purple, 1.0);
    }
}