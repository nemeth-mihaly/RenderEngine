#version 460 core

in layout(location = 0) vec3 a_Pos;
in layout(location = 1) vec3 a_Normal;
in layout(location = 2) vec2 a_Uv;
in layout(location = 3) vec3 a_WorldPos;

uniform mat4 u_WorldView;
uniform mat4 u_WorldProjection;

out vec2 v_Uv;
out vec3 v_DebugWorldPos;

void main()
{
    vec3 cameraRight = vec3(u_WorldView[0][0], u_WorldView[1][0], u_WorldView[2][0]);
    vec3 cameraUp = vec3(u_WorldView[0][1], u_WorldView[1][1], u_WorldView[2][1]);
    vec3 pos = a_WorldPos + a_Pos.x * cameraRight + a_Pos.y * cameraUp;
    gl_Position = u_WorldProjection * u_WorldView * vec4(pos, 1.0);
    v_Uv = a_Uv;
    v_DebugWorldPos = a_WorldPos;
}