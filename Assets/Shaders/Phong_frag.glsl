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
in vec2 v_Uv;

uniform sampler2D u_Texture;
uniform int u_bUseTexture;

uniform Material u_Material;
uniform LightProperties u_LightProps;

uniform vec3 u_Color;

uniform vec3 u_LightColor;
uniform vec3 u_LightPos;

uniform vec3 u_ViewPos;

void main()
{
    // Ambient
    vec3 ambient = u_Material.Ambient * u_LightProps.Ambient;

    // Diffuse
    vec3 normal = normalize(v_Normal);
    vec3 lightDir = normalize(u_LightProps.Pos - v_FragmentPos);
    // If the angle between the normal and the light's position is greater than 90 degrees, 
    // the dot() function produces a negative value. To resolve this issue, use: max(dot(), 0.0).
    vec3 diffuse = (u_Material.Diffuse * max(dot(normal, lightDir), 0.0)) * u_LightProps.Diffuse; 

    // Specular
    vec3 viewDir = normalize(u_ViewPos - v_FragmentPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 specular = (u_Material.Specular * pow(max(dot(viewDir, reflectDir), 0.0), u_Material.Power)) * u_LightProps.Specular;

    // Emissive
    vec3 emissive = u_Material.Emissive;

    // Final color
    vec4 outputColor;
    
    if (u_bUseTexture == 1)
    {
        outputColor = texture(u_Texture, v_Uv) * vec4((ambient + diffuse + specular + emissive), 1.0);

        if (outputColor.a < 0.1)
        {
            discard;
        }
    }
    else
    {
        outputColor = vec4((ambient + diffuse + specular + emissive), 1.0);
    }

    v_FragmentColor = outputColor;
}