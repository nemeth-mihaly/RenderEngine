#version 460 core

struct Material
{
    vec4    ambient;
    vec4    diffuse;
    vec4    specular;
    vec4    emissive;
    float   specularPower;
    int     bUseTex;
};

struct Light
{
    int         type;
    vec3        position;
    vec3        direction;
    vec4        ambient;
    vec4        diffuse;
    vec4        specular;
    float       range;
    float       fallOff;
    float       constantAttenuation;
    float       linearAttenuation;
    float       quadraticAttenuation;
    float       theta;
    float       phi;
};

#define LightType_Directional   0
#define LightType_Point         1
#define LightType_Spot          2

#define MAX_LIGHTS 32

out vec4 fragColor;

in DATA
{   
    vec3    color;
    vec3    normal;
    vec2    uv;
    vec3    fragPosition;
} fragIn;

uniform sampler2D uTex;
uniform Material uMat;

uniform vec3 uViewPosition;
uniform Light uLights[MAX_LIGHTS];
uniform int uNumLights;

float ComputeAttenuation(Light light, float _distance)
{
    return (1.0 / (light.constantAttenuation + light.linearAttenuation * _distance + light.quadraticAttenuation * (_distance * _distance)));
}

vec3 ComputeDiffuse(Light light, vec3 lightDirection, vec3 normal)
{
    // If the angle between the normal and the light's position is greater than 90 degrees, 
    // the dot() function produces a negative value. To resolve this issue, use: max(dot(), 0.0).
    return ((max(dot(normal, lightDirection), 0.0)) * vec3(light.diffuse));
}

vec3 ComputeSpecular(Light light, vec3 viewDirection, vec3 lightDirection, vec3 normal)
{
    vec3 reflectDirection = reflect(-lightDirection, normal);
    return (pow(max(dot(viewDirection, reflectDirection), 0.0), uMat.specularPower) * vec3(light.specular));
}

struct LightingResult
{
    vec4 diffuse;
    vec4 specular;
};

LightingResult ComputeDirectionalLight(Light light, vec3 viewDirection, vec3 normal)
{
    LightingResult result;
    result.diffuse = vec4(0, 0, 0, 0);
    result.specular = vec4(0, 0, 0, 0);

    vec3 lightDirection = normalize(-light.direction);  

    result.diffuse += vec4(ComputeDiffuse(light, lightDirection, normal), 1);
    result.specular += vec4(ComputeSpecular(light, viewDirection, lightDirection, normal), 1);

    return result;
}

LightingResult ComputePointLight(Light light, vec3 viewDirection, vec3 normal)
{
    LightingResult result;
    result.diffuse = vec4(0, 0, 0, 0);
    result.specular = vec4(0, 0, 0, 0);

    vec3 lightDirection = normalize(light.position - fragIn.fragPosition);  

    float _distance = length(light.position - fragIn.fragPosition);
    float attenuation = ComputeAttenuation(light, _distance);

    result.diffuse += vec4(ComputeDiffuse(light, lightDirection, normal), 1.0) * attenuation;
    result.specular += vec4(ComputeSpecular(light, viewDirection, lightDirection, normal), 1.0) * attenuation;

    return result;
}

LightingResult ComputeSpotLight(Light light, vec3 viewDirection, vec3 normal)
{
    LightingResult result;
    result.diffuse = vec4(0, 0, 0, 0);
    result.specular = vec4(0, 0, 0, 0);

    vec3 lightDirection = normalize(light.position - fragIn.fragPosition);
    float theta = dot(lightDirection, normalize(-light.direction));
    float epsilon = light.fallOff - light.phi;
    float intensity = clamp((theta - light.phi) / epsilon, 0.0, 1.0); 

    float _distance = length(light.position - fragIn.fragPosition);
    float attenuation = ComputeAttenuation(light, _distance);

    result.diffuse += vec4(ComputeDiffuse(light, lightDirection, normal), 1.0) * attenuation * intensity;
    result.specular += vec4(ComputeSpecular(light, viewDirection, lightDirection, normal), 1.0) * attenuation * intensity;

    return result;
}

LightingResult ComputeLighting()
{
    LightingResult result;
    result.diffuse = vec4(0, 0, 0, 0);
    result.specular = vec4(0, 0, 0, 0);

    vec3 normal = normalize(fragIn.normal);
    vec3 viewDirection = normalize(uViewPosition - fragIn.fragPosition);

    for (int i = 0; i < uNumLights; i++)
    {
        Light light = uLights[i];

        switch (light.type)
        {
            case LightType_Directional:
            {
                LightingResult r = ComputeDirectionalLight(light, viewDirection, normal);
                result.diffuse += r.diffuse;
                result.specular += r.specular;

                break;
            }

            case LightType_Point:
            {
                LightingResult r = ComputePointLight(light, viewDirection, normal);
                result.diffuse += r.diffuse;
                result.specular += r.specular;

                break;
            }

            case LightType_Spot:
            {
                LightingResult r = ComputeSpotLight(light, viewDirection, normal);
                result.diffuse += r.diffuse;
                result.specular += r.specular;   
 
                break;
            }
        }
    }

    result.diffuse = clamp(result.diffuse, vec4(0, 0, 0, 0), vec4(1, 1, 1, 1));
    result.specular = clamp(result.specular, vec4(0, 0, 0, 0), vec4(1, 1, 1, 1));

    return result;
}

void main()
{
    LightingResult ltColor = ComputeLighting();

    vec4 texColor = vec4(1, 1, 1, 1);

    if (uMat.bUseTex == 1)
    {
        texColor = texture(uTex, fragIn.uv);
    }

    vec4 a = uMat.ambient * vec4(0.2);
    vec4 d = uMat.diffuse * ltColor.diffuse;
    vec4 s = uMat.specular * ltColor.specular;
    vec4 e = uMat.emissive;

    fragColor = vec4(a + d + s + e) * texColor * vec4(fragIn.color, 1);;
}