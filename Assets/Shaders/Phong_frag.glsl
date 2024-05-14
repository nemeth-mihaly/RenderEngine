#version 460 core
out vec4 v_FragmentColor;

struct Material
{
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    vec3 Emissive;
    float Power;
};

struct LightProperties
{
    vec3 Pos;

    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
};

in vec3 v_FragmentPos;
in vec3 v_Normal;

uniform Material u_Material;
uniform LightProperties u_LightProps;

uniform vec3 u_Color;

uniform vec3 u_LightColor;
uniform vec3 u_LightPos;

uniform vec3 u_ViewPos;

void main()
{
    vec3 ambient = u_Material.Ambient * u_LightProps.Ambient;

    vec3 normal = normalize(v_Normal);
    vec3 lightDir = normalize(u_LightProps.Pos - v_FragmentPos);
    // If the angle between the normal and the light's position is greater than 90 degrees, 
    // the dot() function produces a negative value. To resolve this issue, use: max(dot(), 0.0).
    vec3 diffuse = (u_Material.Diffuse * max(dot(normal, lightDir), 0.0)) * u_LightProps.Diffuse; 

    float specSharpness = 32;
    vec3 viewDir = normalize(u_ViewPos - v_FragmentPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 specular = (u_Material.Specular * pow(max(dot(viewDir, reflectDir), 0.0), u_Material.Power)) * u_LightProps.Specular;

    vec3 emissive = u_Material.Emissive;

    vec3 color = (ambient + diffuse + specular + emissive); // * u_Color;
    v_FragmentColor = vec4(color, 1.0);
}