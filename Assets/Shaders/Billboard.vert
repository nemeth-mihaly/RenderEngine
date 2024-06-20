#version 460 core

in layout(location = 0) vec3 a_Pos;
in layout(location = 1) vec3 a_Normal;
in layout(location = 2) vec2 a_Uv;
in layout(location = 3) vec3 a_WorldPos;

layout (std140) uniform Matrices
{
    mat4 Projection;
    mat4 View;
};
 
out vec2 v_Uv;
out vec3 v_DebugWorldPos;

uniform vec3 u_Position;
uniform float scale;

void main()
{
    vec3 cameraRight = vec3(View[0][0], View[1][0], View[2][0]);
    vec3 cameraUp = vec3(View[0][1], View[1][1], View[2][1]);
    //vec3 pos = a_WorldPos + a_Pos.x * cameraRight + a_Pos.y * cameraUp;
    vec3 scaledPos = a_Pos * scale;
    vec3 pos = u_Position + scaledPos.x * cameraRight + scaledPos.y * cameraUp;
    gl_Position = Projection * View * vec4(pos, 1.0);
    v_Uv = a_Uv;
    v_DebugWorldPos = a_WorldPos;
}