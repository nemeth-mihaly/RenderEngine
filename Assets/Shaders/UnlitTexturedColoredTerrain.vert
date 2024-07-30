#version 460 core

in layout (location = 0) vec3 aPos;
in layout (location = 1) vec3 aColor;
in layout (location = 2) vec2 aUv;

out DATA
{   
    vec3    pos;
    vec3    color;
    vec2    uv;
} vertOut;

uniform mat4 uModel, uView, uProjection;

void main()
{
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);

    vertOut.pos = aPos;
    vertOut.color = aColor;
    vertOut.uv = aUv;
}