#version 430 core

struct Material
{
    sampler2D t_diffuse1;
    sampler2D t_diffuse2;
    sampler2D t_diffuse3;
    sampler2D t_diffuse4;

    sampler2D t_specular1;
    sampler2D t_specular2;
    sampler2D t_specular3;
    sampler2D t_specular4;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct DirectionalLight
{
    vec3 dir;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight
{
    vec3 pos;
    vec3 dir;
    float cutoff;
    float outerCutoff;

    vec3 diffuse;
    vec3 specular;

    vec3 clq;
};

uniform Material u_material;
uniform DirectionalLight u_dirLight;
// uniform SpotLight u_spotLight;
uniform bool u_enableLighting;
uniform vec3 u_cameraPos;

in vec2 v_textureUv;
in vec3 v_norm;
in vec3 v_uv;

out vec4 resultColor;

vec4 calculateDirLight(DirectionalLight light, vec4 t_diff, vec4 t_spec)
{
    vec3 cameraToVertex = normalize(v_uv - u_cameraPos);
    vec3 dir = normalize(light.dir);

    vec4 ambient = vec4(light.ambient, 1.0) * t_diff;
    vec4 diffuse = vec4(light.diffuse, 1.0) * max(dot(v_norm, -dir), 0.0) * t_diff;

    vec3 reflectDir = reflect(dir, v_norm);
    float specComp = pow(max(dot(reflectDir, -cameraToVertex), 0.0), u_material.shininess);
    vec4 specular = vec4(light.specular, 1.0) * specComp * t_spec;

    return (ambient + diffuse + specular);
}

// vec3 calculateSpotLight(SpotLight light, vec3 t_diff, vec3 t_spec)
// {
//     float dist = length(v_uv - light.pos);
//     float attenuation = 1.0 / (light.clq.x + light.clq.y * dist + light.clq.z * (dist * dist));
//     vec3 cameraToVertex = normalize(v_uv - u_cameraPos);
//     vec3 lightToVertex = normalize(v_uv - light.pos);

//     float theta = dot(lightToVertex, light.dir);
//     float eps = light.cutoff - light.outerCutoff;
//     float intensity = clamp((theta - light.outerCutoff) / eps, 0.0, 1.0);

//     vec3 diffuse = light.diffuse * max(dot(v_norm, -light.dir), 0.0) * t_diff * attenuation * intensity;

//     vec3 reflectDir = reflect(normalize(light.dir), v_norm);
//     float specComp = pow(max(dot(reflectDir, -cameraToVertex), 0.0), u_material.shininess);
//     vec3 specular = light.specular * specComp * t_spec * attenuation * intensity;

//     return (diffuse + specular);
// }

void main()
{
    vec4 r = vec4(0.0);
    if (u_enableLighting)
    {
        vec4 t_diff = texture(u_material.t_diffuse1, v_textureUv);
        vec4 t_spec = texture(u_material.t_specular1, v_textureUv);
        r += calculateDirLight(u_dirLight, t_diff, t_spec);
        // r += calculateSpotLight(u_spotLight, t_diff, t_spec);
    }
    else {
        r = texture(u_material.t_diffuse1, v_textureUv);
    }
    resultColor = r;
}