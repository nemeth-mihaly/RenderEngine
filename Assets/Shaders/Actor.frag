#version 460 core

out vec4 fragColor;

in DATA
{
    vec3    Pos;
    vec3    Color;
    vec2    Uv;
} data;

uniform sampler2D uTexture;

void main()
{
    fragColor = vec4(data.Color, 1.0);
    fragColor *= texture(uTexture, data.Uv);
}