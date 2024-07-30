#version 460 core

out vec4 fragColor;

in DATA
{   
    vec3    pos;
    vec3    color;
    vec2    uv;
} fragIn;

uniform sampler2D uBlendmapTex; // 0

uniform sampler2D uTilesetTex1; // 1
uniform sampler2D uTilesetTex2; // 2
uniform sampler2D uTilesetTex3; // 3
// uniform sampler2D uTilesetTex3;

void main()
{
    vec2 uv;
    uv.x = fragIn.uv.x * 16.0;
    uv.y = fragIn.uv.y * 16.0;

    float colorValue = fragIn.pos.y + 4.0;
    colorValue /= 96.0;

    vec4 blendmapColor = texture(uBlendmapTex, fragIn.uv);
    float blendmapColorAmount = 1.0 - (blendmapColor.r + blendmapColor.g + blendmapColor.b);

    vec4 tilesetColor1 = texture(uTilesetTex1, uv) * blendmapColorAmount;
    vec4 tilesetColor2 = texture(uTilesetTex2, uv) * blendmapColor.r;
    vec4 tilesetColor3 = texture(uTilesetTex3, uv) * blendmapColor.g;

    fragColor = (tilesetColor1 + tilesetColor2 + tilesetColor3); + vec4(colorValue, colorValue, colorValue, 1.0);
}