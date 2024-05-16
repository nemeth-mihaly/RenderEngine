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
    return (1.0 / (light.ConstantAttenuation + light.LinearAttenuation * _distance + light.QuadraticAttenuation * (_distance * _distance)));
}

vec3 ComputeDiffuse(Light light, vec3 lightDirection, vec3 normal)
{
    // If the angle between the normal and the light's position is greater than 90 degrees, 
    // the dot() function produces a negative value. To resolve this issue, use: max(dot(), 0.0).
    return ((max(dot(normal, lightDirection), 0.0)) * light.Diffuse);
}

vec3 ComputeSpecular(Light light, vec3 viewDirection, vec3 lightDirection, vec3 normal)
{
    vec3 reflectDirection = reflect(-lightDirection, normal);
    return (pow(max(dot(viewDirection, reflectDirection), 0.0), u_Material.Power) * light.Specular);
}

struct LightingResult
{
    vec3 Diffuse;
    vec3 Specular;
};

LightingResult ComputeDirectionalLight(Light light, vec3 viewDirection, vec3 normal)
{
    LightingResult result;
    result.Diffuse = vec3(0, 0, 0);
    result.Specular = vec3(0, 0, 0);

    vec3 lightDirection = normalize(-light.Direction);  

    result.Diffuse += ComputeDiffuse(light, lightDirection, normal);
    result.Specular += ComputeSpecular(light, viewDirection, lightDirection, normal);

    return result;
}

LightingResult ComputePointLight(Light light, vec3 viewDirection, vec3 normal)
{
    LightingResult result;
    result.Diffuse = vec3(0, 0, 0);
    result.Specular = vec3(0, 0, 0);

    vec3 lightDirection = normalize(light.Position - v_FragmentPos);  

    float _distance = length(light.Position - v_FragmentPos);
    float attenuation = ComputeAttenuation(light, _distance);

    result.Diffuse += ComputeDiffuse(light, lightDirection, normal) * attenuation;
    result.Specular += ComputeSpecular(light, viewDirection, lightDirection, normal) * attenuation;

    return result;
}

LightingResult ComputeSpotLight(Light light, vec3 viewDirection, vec3 normal)
{
    LightingResult result;
    result.Diffuse = vec3(0, 0, 0);
    result.Specular = vec3(0, 0, 0);

    vec3 lightDirection = normalize(light.Position - v_FragmentPos);
    float theta = dot(lightDirection, normalize(-light.Direction)); 

    if (theta > light.Falloff)
    {
        float _distance = length(light.Position - v_FragmentPos);
        float attenuation = ComputeAttenuation(light, _distance);

        result.Diffuse += ComputeDiffuse(light, lightDirection, normal) * attenuation;
        result.Specular += ComputeSpecular(light, viewDirection, lightDirection, normal) * attenuation;
    }

    return result;
}

LightingResult ComputeLighting()
{
    LightingResult result;
    result.Diffuse = vec3(0, 0, 0);
    result.Specular = vec3(0, 0, 0);

    vec3 normal = normalize(v_Normal);
    vec3 viewDirection = normalize(u_ViewPos - v_FragmentPos);

    // Compute lights.
    for (int i = 0; i < MAX_LIGHTS; ++i)
    {
        Light light = u_Lights[i];

        switch (light.Type)
        {
            case LightType_Directional:
            {
                LightingResult r = ComputeDirectionalLight(light, viewDirection, normal);
                result.Diffuse += r.Diffuse;
                result.Specular += r.Specular;

                break;
            }

            case LightType_Point:
            {
                LightingResult r = ComputePointLight(light, viewDirection, normal);
                result.Diffuse += r.Diffuse;
                result.Specular += r.Specular;

                break;
            }

            case LightType_Spot:
            {
                LightingResult r = ComputeSpotLight(light, viewDirection, normal);
                result.Diffuse += r.Diffuse;
                result.Specular += r.Specular;   
 
                break;
            }
        }
    }

    result.Diffuse = clamp(result.Diffuse, vec3(0, 0, 0), vec3(1, 1, 1));
    result.Specular = clamp(result.Specular, vec3(0, 0, 0), vec3(1, 1, 1));

    return result;
}

void main()
{
    LightingResult litColor = ComputeLighting();

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

    vec3 ambient = u_Material.Ambient * vec3(0.2, 0.2, 0.2);
    vec3 diffuse = u_Material.Diffuse * litColor.Diffuse;
    vec3 specular = u_Material.Specular * litColor.Specular;
    vec3 emissive = u_Material.Emissive;

    // Final color.
    vec4 finalColor = vec4((ambient + diffuse + specular + emissive), 1.0) * texColor;

    // End.
    v_FragmentColor = finalColor;
}