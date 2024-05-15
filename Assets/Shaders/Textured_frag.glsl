#version 460 core
out vec4 v_FragmentColor;

in vec2 v_Uv;

uniform sampler2D u_Texture;
uniform int u_bUseTexture;

void main()
{
    vec4 outputColor;

    if (u_bUseTexture == 1)
    {
        outputColor = texture(u_Texture, v_Uv);
        
        if (outputColor.a < 0.1)
        {
            discard;
        }
    }
    else
    {
        outputColor = vec4(1.0, 0.0, 1.0, 1.0);
    }

    v_FragmentColor = outputColor;
}