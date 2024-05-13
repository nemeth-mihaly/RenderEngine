#version 460 core
out vec4 v_FragmentColor;

in vec3 v_Uv;

uniform samplerCube u_Texture;

void main()
{
    v_FragmentColor = texture(u_Texture, v_Uv);
}