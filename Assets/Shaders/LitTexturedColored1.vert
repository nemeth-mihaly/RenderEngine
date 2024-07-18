#version 460 core

in layout (location = 0) vec3 aPos;
in layout (location = 1) vec3 aNorm;
in layout (location = 2) vec3 aColor;
in layout (location = 3) vec2 aUv;

out DATA
{   
    vec3    color;
    vec3    normal;
    vec2    uv;
    vec3    fragPosition;
} vertOut;

uniform mat4 uModel, uView, uProjection;

void main()
{
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);

    vertOut.normal = mat3(transpose(inverse(uModel))) * aNorm;
    vertOut.color = aColor;
    vertOut.uv = aUv;
    vertOut.fragPosition = vec3(uModel * vec4(aPos, 1.0));
}