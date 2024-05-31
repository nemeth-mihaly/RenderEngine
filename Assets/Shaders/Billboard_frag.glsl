#version 460 core
out vec4 v_FragmentColor;

in vec2 v_Uv;
in vec3 v_DebugWorldPos;

uniform sampler2D u_Texture;

void main()
{
    vec4 color = texture(u_Texture, v_Uv);
    v_FragmentColor = vec4(v_DebugWorldPos, 1.0);
}