#version 460 core

in layout(location = 0) vec3 a_Pos;
in layout(location = 1) vec3 a_Normal;
in layout(location = 2) vec2 a_Uv;

layout (std140, binding = 0) uniform Matrices
{
    mat4 Projection;
    mat4 View;
};

uniform mat4 u_World;

out vec3 v_FragmentPos;
out vec3 v_Normal;
out vec2 v_Uv;

void main()
{
    vec4 worldPos = u_World * vec4(a_Pos, 1.0);
    gl_Position = Projection * View * worldPos;
    v_FragmentPos = vec3(worldPos);
    v_Normal = mat3(transpose(inverse(u_World))) * a_Normal; // TODO: Move the calculation to the CPU.
    v_Uv = a_Uv;
}