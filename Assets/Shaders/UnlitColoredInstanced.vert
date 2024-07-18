#version 460 core

in layout (location = 0) vec3 aPos;
in layout (location = 1) vec3 aColor;
in layout (location = 2) mat4 aModel;

out DATA
{   
    vec3    color;
} vertOut;

uniform mat4 uModel, uView, uProjection;

void main()
{
    gl_Position = uProjection * uView * aModel * vec4(aPos, 1.0);

    vertOut.color = aColor;
}