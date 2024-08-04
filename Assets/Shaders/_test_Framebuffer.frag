#version 460 core

out vec4 fragColor;

in DATA
{   
    vec3    color;
    vec2    uv;
} fragIn;

uniform sampler2D uTex;

void main()
{
    vec4 color = vec4(vec3(texture(uTex, fragIn.uv)), 1);
    fragColor = color * vec4(fragIn.color, 1);
}