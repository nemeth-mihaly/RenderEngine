#version 460 core
out vec4 v_FragmentColor;

// --- Material ---------------------------------

struct Material
{
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    vec3 Emissive;
    float Power;
    int bUseTexture;
};

uniform Material u_Material;

uniform sampler2D u_Texture;

// ----------------------------------------------

// --- Lights -----------------------------------

struct Light
{
    int Type;
    vec3 Position;
    vec3 Direction;
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    float Range;
    float Falloff;
    float ConstantAttenuation;
    float LinearAttenuation;
    float QuadraticAttenuation;
    float Theta;
    float Phi;
};

#define LightType_Directional 0
#define LightType_Point 1
#define LightType_Spot 2

#define MAX_LIGHTS 32

uniform Light u_Lights[MAX_LIGHTS];

// ----------------------------------------------

in vec3 v_FragmentPos;
in vec3 v_Normal;
in vec2 v_Uv;

uniform vec3 u_ViewPos;

float ComputeAttenuation(Light light, float _distance)
{
    //float _distance = length(light.Position - v_FragmentPos);
    return (1.0 / (light.ConstantAttenuation + light.LinearAttenuation * _distance + light.QuadraticAttenuation * (_distance * _distance)));
}

void main()
{
    // Result color.
    vec3 litDiffuse = vec3(0, 0, 0);
    vec3 litSpecular = vec3(0, 0, 0);

    // Compute lights.
    for (int i = 0; i < MAX_LIGHTS; ++i)
    {
        switch (u_Lights[i].Type)
        {
            case LightType_Directional:
            {
                // Ambient.
                //vec3 ambient = u_Material.Ambient * u_Lights[i].Ambient;

                // Diffuse.
                vec3 normal = normalize(v_Normal);

                vec3 lightDir = normalize(-u_Lights[i].Direction);  
                // If the angle between the normal and the light's position is greater than 90 degrees, 
                // the dot() function produces a negative value. To resolve this issue, use: max(dot(), 0.0).
                vec3 diffuse = (u_Material.Diffuse * max(dot(normal, lightDir), 0.0)) * u_Lights[i].Diffuse; 

                // Specular.
                vec3 viewDir = normalize(u_ViewPos - v_FragmentPos);
                vec3 reflectDir = reflect(-lightDir, normal);
                vec3 specular = (u_Material.Specular * pow(max(dot(viewDir, reflectDir), 0.0), u_Material.Power)) * u_Lights[i].Specular;

                // Emissive.
                //vec3 emissive = u_Material.Emissive;

                // End.
                litDiffuse += diffuse;
                litSpecular += specular;

                break;
            }

            case LightType_Point:
            {
                // Ambient.
                //vec3 ambient = u_Material.Ambient * u_Lights[i].Ambient;

                // Diffuse.
                vec3 normal = normalize(v_Normal);

                vec3 lightDir = normalize(u_Lights[i].Position - v_FragmentPos);
                // If the angle between the normal and the light's position is greater than 90 degrees, 
                // the dot() function produces a negative value. To resolve this issue, use: max(dot(), 0.0).
                vec3 diffuse = (u_Material.Diffuse * max(dot(normal, lightDir), 0.0)) * u_Lights[i].Diffuse; 

                // Specular.
                vec3 viewDir = normalize(u_ViewPos - v_FragmentPos);
                vec3 reflectDir = reflect(-lightDir, normal);
                vec3 specular = (u_Material.Specular * pow(max(dot(viewDir, reflectDir), 0.0), u_Material.Power)) * u_Lights[i].Specular;

                // Attenuation.
                float _distance = length(u_Lights[i].Position - v_FragmentPos);
                float attenuation = ComputeAttenuation(u_Lights[i], _distance);

                diffuse *= attenuation;
                specular *= attenuation;
        
                // End.
                litDiffuse += diffuse;
                litSpecular += specular;

                break;
            }

            case LightType_Spot:
            {
                vec3 normal = normalize(v_Normal);

                vec3 lightDir = normalize(u_Lights[i].Position - v_FragmentPos);
                float theta = dot(lightDir, normalize(-u_Lights[i].Direction)); 
                
                if (theta > u_Lights[i].Falloff)
                {
                    // Diffuse.
                    // If the angle between the normal and the light's position is greater than 90 degrees, 
                    // the dot() function produces a negative value. To resolve this issue, use: max(dot(), 0.0).
                    vec3 diffuse = (u_Material.Diffuse * max(dot(normal, lightDir), 0.0)) * u_Lights[i].Diffuse; 

                    // Specular.
                    vec3 viewDir = normalize(u_ViewPos - v_FragmentPos);
                    vec3 reflectDir = reflect(-lightDir, normal);
                    vec3 specular = (u_Material.Specular * pow(max(dot(viewDir, reflectDir), 0.0), u_Material.Power)) * u_Lights[i].Specular;

                    // Attenuation.
                    float _distance = length(u_Lights[i].Position - v_FragmentPos);
                    float attenuation = ComputeAttenuation(u_Lights[i], _distance);

                    diffuse *= attenuation;
                    specular *= attenuation;

                    // End
                    litDiffuse += diffuse;
                    litSpecular += specular;
                }
                else
                {
                    // Diffuse.
                    // If the angle between the normal and the light's position is greater than 90 degrees, 
                    // the dot() function produces a negative value. To resolve this issue, use: max(dot(), 0.0).
                    vec3 diffuse = vec3(0, 0, 0);

                    // Specular.
                    vec3 specular = vec3(0, 0, 0);

                    // Emissive.
                    //vec3 emissive = u_Material.Emissive;

                    // End.
                    litDiffuse += diffuse;
                    litSpecular += specular;
                }

                break;
            }
        }
    }

    // Texture.
    vec4 texColor = vec4(1, 1, 1, 1);

    if (u_Material.bUseTexture == 1)
    {
        texColor = texture(u_Texture, v_Uv);

        if (texColor.a < 0.1)
        {
            discard;
        }
    }

    // Final color.
    vec4 finalColor = vec4(((u_Material.Ambient * 0.2) + litDiffuse + litSpecular + u_Material.Emissive), 1.0) * texColor;

    // End.
    v_FragmentColor = finalColor;
}