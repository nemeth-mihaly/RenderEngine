#version 460 core

struct Material
{
    vec4    ambient;
    vec4    diffuse;
    vec4    specular;
    vec4    emissive;
    float   specularPower;
    int     bUseTex;
};

out vec4 fragColor;

in DATA
{   
    vec3    color;
    vec2    uv;
} fragIn;

uniform sampler2D uTex;
uniform Material uMat;

void main()
{
    vec4 texColor = vec4(1, 1, 1, 1);

    if (uMat.bUseTex == 1)
    {
        texColor = texture(uTex, fragIn.uv);
    }

    fragColor = vec4(uMat.ambient + uMat.diffuse + uMat.specular + uMat.emissive) * texColor * vec4(fragIn.color, 1);;
}