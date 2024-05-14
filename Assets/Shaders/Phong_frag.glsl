#version 460 core
out vec4 v_FragmentColor;

in vec3 v_FragmentPos;
in vec3 v_Normal;

uniform vec3 u_Color;

uniform vec3 u_LightColor;
uniform vec3 u_LightPos;

uniform vec3 u_ViewPos;

void main()
{
    vec3 ambient = 0.2 * u_LightColor;

    vec3 normal = normalize(v_Normal);
    vec3 lightDir = normalize(u_LightPos - v_FragmentPos);
    // If the angle between the normal and the light's position is greater than 90 degrees, 
    // the dot() function produces a negative value. To resolve this issue, use: max(dot(), 0.0).
    vec3 diffuse = max(dot(normal, lightDir), 0.0) * u_LightColor; 

    float specSharpness = 32;
    vec3 viewDir = normalize(u_ViewPos - v_FragmentPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 specular = 0.5 * pow(max(dot(viewDir, reflectDir), 0.0), specSharpness) * u_LightColor;

    vec3 color = (ambient + diffuse + specular) * u_Color;
    v_FragmentColor = vec4(color, 1.0);
}