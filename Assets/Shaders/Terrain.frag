#version 460 core
out vec4 v_FragmentColor;

// --- Material ---------------------------------

struct Material
{
    vec4 Ambient;
    vec4 Diffuse;
    vec4 Specular;
    vec4 Emissive;
    float SpecularPower;
    int bUseTexture;
};

uniform Material u_Material;

uniform sampler2D u_BlendMapTexture;
uniform sampler2D u_DirtBaseTexture;
uniform sampler2D u_StonebrickTexture;
uniform sampler2D u_GrassTexture;

// ----------------------------------------------

// --- Lights -----------------------------------

struct Light
{
    int Type;
    vec3 Position;
    vec3 Direction;
    vec4 Ambient;
    vec4 Diffuse;
    vec4 Specular;
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
    return ((max(dot(normal, lightDirection), 0.0)) * vec3(light.Diffuse));
}

vec3 ComputeSpecular(Light light, vec3 viewDirection, vec3 lightDirection, vec3 normal)
{
    vec3 reflectDirection = reflect(-lightDirection, normal);
    return (pow(max(dot(viewDirection, reflectDirection), 0.0), u_Material.SpecularPower) * vec3(light.Specular));
}

struct LightingResult
{
    vec4 Diffuse;
    vec4 Specular;
};

LightingResult ComputeDirectionalLight(Light light, vec3 viewDirection, vec3 normal)
{
    LightingResult result;
    result.Diffuse = vec4(0, 0, 0, 0);
    result.Specular = vec4(0, 0, 0, 0);

    vec3 lightDirection = normalize(-light.Direction);  

    result.Diffuse += vec4(ComputeDiffuse(light, lightDirection, normal), 1);
    result.Specular += vec4(ComputeSpecular(light, viewDirection, lightDirection, normal), 1);

    return result;
}

LightingResult ComputePointLight(Light light, vec3 viewDirection, vec3 normal)
{
    LightingResult result;
    result.Diffuse = vec4(0, 0, 0, 0);
    result.Specular = vec4(0, 0, 0, 0);

    vec3 lightDirection = normalize(light.Position - v_FragmentPos);  

    float _distance = length(light.Position - v_FragmentPos);
    float attenuation = ComputeAttenuation(light, _distance);

    result.Diffuse += vec4(ComputeDiffuse(light, lightDirection, normal), 1.0) * attenuation;
    result.Specular += vec4(ComputeSpecular(light, viewDirection, lightDirection, normal), 1.0) * attenuation;

    return result;
}

LightingResult ComputeSpotLight(Light light, vec3 viewDirection, vec3 normal)
{
    LightingResult result;
    result.Diffuse = vec4(0, 0, 0, 0);
    result.Specular = vec4(0, 0, 0, 0);

    vec3 lightDirection = normalize(light.Position - v_FragmentPos);
    float theta = dot(lightDirection, normalize(-light.Direction));
    float epsilon = light.Falloff - light.Phi;
    float intensity = clamp((theta - light.Phi) / epsilon, 0.0, 1.0); 

    float _distance = length(light.Position - v_FragmentPos);
    float attenuation = ComputeAttenuation(light, _distance);

    result.Diffuse += vec4(ComputeDiffuse(light, lightDirection, normal), 1.0) * attenuation * intensity;
    result.Specular += vec4(ComputeSpecular(light, viewDirection, lightDirection, normal), 1.0) * attenuation * intensity;

    return result;
}

LightingResult ComputeLighting()
{
    LightingResult result;
    result.Diffuse = vec4(0, 0, 0, 0);
    result.Specular = vec4(0, 0, 0, 0);

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

    result.Diffuse = clamp(result.Diffuse, vec4(0, 0, 0, 0), vec4(1, 1, 1, 1));
    result.Specular = clamp(result.Specular, vec4(0, 0, 0, 0), vec4(1, 1, 1, 1));

    return result;
}

void main()
{
    LightingResult litColor = ComputeLighting();

    // Texture.
    vec4 texColor = vec4(1, 1, 1, 1);

    if (u_Material.bUseTexture == 1)
    {
        vec4 blendMapTexColor = texture(u_BlendMapTexture, v_Uv);
        float blendMapColorAmount = 1 - (blendMapTexColor.r + blendMapTexColor.g + blendMapTexColor.b);

        vec4 dirtBaseTex = texture(u_DirtBaseTexture, v_Uv * 40.0) * blendMapColorAmount;
        vec4 stonebricksTex = texture(u_StonebrickTexture, v_Uv * 40.0) * blendMapTexColor.b;
        vec4 grassTex = texture(u_GrassTexture, v_Uv * 40.0) * blendMapTexColor.g;

        texColor = dirtBaseTex + stonebricksTex + grassTex;

        if (texColor.a < 0.01)
        {
            discard;
        }
    }

    vec4 globalAmbient = vec4(0.2, 0.2, 0.2, 1.0);
    vec4 ambient = u_Material.Ambient * globalAmbient;
    vec4 diffuse = u_Material.Diffuse * litColor.Diffuse;
    vec4 specular = u_Material.Specular * litColor.Specular;
    vec4 emissive = u_Material.Emissive;

    // Final color.
    vec4 finalColor = vec4((ambient + diffuse + specular + emissive)) * texColor;

    // End.
    v_FragmentColor = finalColor;
}