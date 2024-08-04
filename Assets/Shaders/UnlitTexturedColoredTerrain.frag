// https://madebyevan.com/shaders/grid/

#version 460 core

out vec4 fragColor;

in DATA
{   
    vec3    pos;
    vec3    color;
    vec2    uv;
} fragIn;

uniform vec3 uBrushPos;
uniform float uBrushRadius;

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

    int bWireframeEnabled = 0;
    if (bWireframeEnabled == 1)
    {
        vec3 wireframeColor = vec3(1, 1, 1);

        vec2 grid = abs(fract(fragIn.pos.xz - 0.5) - 0.5) / fwidth(fragIn.pos.xz);
        grid = step(1.1, grid);

        float wireframeAlphamap = 1.0 - min(min(grid.x, grid.y), 1.0);

        fragColor.rgb = mix(fragColor.rgb, wireframeColor, wireframeAlphamap);
    }

    int bContourEnabled = 0;
    if (bContourEnabled == 1)
    {
        vec3 contourColor = vec3(1, 1, 1);

        float contour = abs(fract(fragIn.pos.y - 0.5) - 0.5) / fwidth(fragIn.pos.y);
        contour = step(1.1, contour);

        float contourAlphamap = 1.0 - min(contour, 1.0);

        fragColor.rgb = mix(fragColor.rgb, contourColor, contourAlphamap);
    }

    vec3 brushColor = vec3(1, 1, 1);
    
    float brush = length(fragIn.pos.xz - uBrushPos.xz);
    float brushAlphamap = step(uBrushRadius - 0.02, brush) - step(uBrushRadius + 0.02, brush);
    brushAlphamap = max(brushAlphamap, step(brush, 0.1));

    fragColor.rgb = mix(fragColor.rgb, brushColor, brushAlphamap);
}