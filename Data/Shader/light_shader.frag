#version 330 core

struct Material
{
    sampler2D t_diffuse0;
    sampler2D t_specular0;
    float shininess;
};

struct SpotLight 
{
    vec3 dir;
    vec3 pos;
    float cutoff;
    float outerCutoff;

    vec3 diffuse;
    vec3 specular;

    vec3 clq;
};

struct PointLight
{
    vec3 pos;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 clq;
};

struct DirectionalLight
{
    vec3 dir;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 u_cameraPos;
uniform Material u_material;

#define POINT_LIGHT_NUM 4
uniform PointLight u_pointLights[POINT_LIGHT_NUM];

uniform DirectionalLight u_dirLight;
uniform SpotLight u_spotLight;

in vec3 v_normal;
in vec3 v_uv;
in vec2 v_textureUv;

out vec4 resultColor;

vec3 dirLightCalculation(DirectionalLight light, vec3 norm, vec3 cameraToVertex)
{
    vec3 normal = normalize(norm);
    vec3 lightDir = normalize(-light.dir);
    
    vec3 ambient = light.ambient * vec3(texture(u_material.diffuse, v_textureUv));
    vec3 diffuse = light.diffuse * max(dot(normal, -light.dir), 0.0) * vec3(texture(u_material.diffuse, v_textureUv));
    
    vec3 reflectDir = reflect(-lightDir, normal);
    float specComp = pow(max(dot(reflectDir, normalize(-cameraToVertex)), 0.0), u_material.shininess);
    vec3 specular = light.specular * specComp * vec3(texture(u_material.specular, v_textureUv));

    return (ambient + diffuse + specular);
}

vec3 pointLightCalculation(PointLight light, vec3 norm, vec3 uv, vec3 cameraToVertex)
{
    vec3 normal = normalize(norm);
    
    vec3 lightDir = normalize(light.pos - uv);
    float distance = length(light.pos - uv);
    float attenuation = 1.0 / (light.clq.x + light.clq.y * distance + light.clq.z * (distance * distance));

    vec3 ambient = light.ambient * vec3(texture(u_material.diffuse, v_textureUv));
    vec3 diffuse = light.diffuse * max(dot(norm, lightDir), 0.0) * vec3(texture(u_material.diffuse, v_textureUv)) * attenuation;
    
    vec3 reflectDir = reflect(-lightDir, norm);
    float specComp = pow(max(dot(reflectDir, normalize(-cameraToVertex)), 0.0), u_material.shininess);
    vec3 specular = light.specular * specComp * vec3(texture(u_material.specular, v_textureUv)) * attenuation;

    return diffuse + specular;
}

vec3 spotLightCalculation(SpotLight light, vec3 norm, vec3 uv, vec3 cameraToVertex)
{
    vec3 normal = normalize(norm);

    vec3 lightDir = normalize(light.pos - uv);
    float distance = length(light.pos - uv);
    float attenuation = 1.0 / (light.clq.x + light.clq.y * distance + light.clq.z * (distance * distance));

    vec3 tdiff = vec3(texture(u_material.diffuse, v_textureUv));
    vec3 tspec = vec3(texture(u_material.specular, v_textureUv));

    float theta = dot(normalize(-light.dir), lightDir);
    float eps = light.cutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / eps, 0.0, 1.0);

    vec3 diffuse = light.diffuse * tdiff * max(dot(lightDir, normal), 0.0) * attenuation * intensity;
    vec3 reflectDir = reflect(-lightDir, normal);
    float specComp = pow(max(dot(reflectDir, normalize(-cameraToVertex)), 0.0), u_material.shininess);
    vec3 specular = light.specular * tspec * specComp * attenuation * intensity;

    return (diffuse + specular);
}

void main()
{
    vec3 viewDir = v_uv - u_cameraPos;

    vec3 result = dirLightCalculation(u_dirLight, v_normal, viewDir);
    for (int i = 0; i < POINT_LIGHT_NUM; i++)
    {
        result += pointLightCalculation(u_pointLights[i], v_normal, v_uv, viewDir);
    }
    result += spotLightCalculation(u_spotLight, v_normal, v_uv, viewDir);
    
    resultColor = vec4(result, 1.0); 
}