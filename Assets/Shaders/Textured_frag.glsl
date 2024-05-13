#version 460 core
out vec4 v_FragmentColor;

in vec2 v_Uv;

uniform sampler2D u_Texture;
uniform int u_bHasTexture;

void main()
{
    if (u_bHasTexture == 1)
    {
        vec4 color = texture(u_Texture, v_Uv);
        
        if (color.a < 0.1)
        {
            discard;
        }

        v_FragmentColor = texture(u_Texture, v_Uv);
    }
    else
    {
        vec3 color = vec3(1.0, 0.0, 1.0); // Purple
        v_FragmentColor = vec4(color, 1.0);
    }
}