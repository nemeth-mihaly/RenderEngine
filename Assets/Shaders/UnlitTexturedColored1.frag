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
    fragColor = texture(uTex, fragIn.uv) * vec4(fragIn.color, 1);
}