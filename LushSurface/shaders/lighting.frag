#version 330 core
struct Material {
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirectionalLight dirLight;

uniform Material material;
uniform SpotLight spot;


out vec4 color;

in vec3 fragPosition;
in vec3 Normal;
in float focusDistance;

uniform vec3 viewPosition;
uniform vec3 focusPosition;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewPos, vec3 viewDir);

void main()
{
    vec3 result = vec3(0.0f, 0.0f, 0.0f);

    vec3 norm = normalize(Normal);
    vec3 viewDirection = normalize(viewPosition - fragPosition);

    result += CalcDirLight(dirLight, norm, viewDirection);
    /*for(int i = 0; i < NR_POINT_LIGHTS; i++) {
        result += CalcPointLight(pointLights[i], norm, fragPosition, viewDirection);
    }*/
    //result += CalcSpotLight(spot, norm, viewPosition, viewDirection);

    float alpha = 1.0f;
    float xdiff1 = fragPosition.x - focusPosition.x;
    float xdiff2 = fragPosition.x - viewPosition.x;
    float zdiff1 = fragPosition.z - focusPosition.z;
    float zdiff2 = fragPosition.z - viewPosition.z;

    if((xdiff1 < 0 && xdiff2 > 0 || xdiff1 > 0 && xdiff2 < 0) && (zdiff1 < 0 && zdiff2 > 0 || zdiff1 > 0 && zdiff2 < 0))
        if(focusDistance < 0)  color.g = 0.9f;

    result.x -= min(focusDistance * 0.01f, 0.15f);
    result.y -= min(focusDistance * 0.01f, 0.15f);
    result.z -= min(focusDistance * 0.01f, 0.15f);

    color = vec4(result, alpha);
}

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDirection = normalize(-light.direction);

    float diff = max(dot(normal, lightDirection), 0.0);

    vec3 reflectDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDir, reflectDirection), 0.0), material.shininess);

    vec3 ambient =  light.ambient * material.diffuse;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;

    return(ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDirection = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDirection), 0.0);

    vec3 reflectDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDir, reflectDirection), 0.0), material.shininess);

    float distance    = length(light.position - fragPosition);
    float attenuation = 1.0f / (light.constant + light.linear * distance +
                light.quadratic * (distance * distance));

    vec3 ambient =  light.ambient * material.diffuse;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewPos, vec3 viewDir) {
    vec3 lightDirection = normalize(light.position - fragPosition);

    float diff = max(dot(normal, lightDirection), 0.0f);
    vec3 reflectDir = reflect(-lightDirection, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    float theta = dot(lightDirection, -light.direction);
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    float distance    = length(light.position - fragPosition);
    float attenuation = 1.0f / (light.constant + light.linear * distance +
                light.quadratic * (distance * distance));

    vec3 ambient =  light.ambient * material.diffuse;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;

    diffuse  *= intensity;
    specular *= intensity;

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}
