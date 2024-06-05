#version 460 core
out vec4 v_FragmentColor;

in vec2 v_Uv;
in vec3 v_DebugWorldPos;

uniform sampler2D u_Texture;

void main()
{
    v_FragmentColor = texture(u_Texture, v_Uv);
}