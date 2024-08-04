#version 460 core

in layout (location = 0) vec3 aPos;
in layout (location = 1) vec3 aColor;
in layout (location = 2) vec2 aUv;

out DATA
{   
    vec3    color;
    vec2    uv;
} vertOut;

uniform mat4 uModel, uView, uProjection;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    vertOut.color = aColor;
    vertOut.uv = aUv;
}