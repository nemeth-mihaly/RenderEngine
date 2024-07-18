#version 460 core

out vec4 fragColor;

in DATA
{   
    vec3    color;
} fragIn;

void main()
{
    fragColor = vec4(fragIn.color, 1.0);
}