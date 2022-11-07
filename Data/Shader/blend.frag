#version 430 core

struct PointLight
{
    vec3 clq;
    vec3 pos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform sampler2D u_texture;
uniform PointLight u_pointLight;
uniform vec3 u_cameraPos;

in vec3 v_uv;
in vec3 v_norm;
in vec2 v_textureUv;

out vec4 resultColor;

vec4 getPointLightComponent(PointLight light, vec4 t_diff, vec4 t_spec)
{
    vec3 uvToLight = normalize(light.pos - v_uv);   
    float dist = length(light.pos - v_uv);
    float attenuation = 1.0 / (light.clq.x + light.clq.y * dist + light.clq.z * dist * dist);

    vec4 ambient = vec4(light.ambient, 1.0) * t_diff * attenuation;
 
    vec3 n = normalize(v_norm);
    vec4 diffuse = vec4(light.diffuse, 1.0) * max(dot(uvToLight, n), 0.0) * t_diff * attenuation;
 
    vec3 reflectDir = reflect(-uvToLight, n);
    vec3 cameraToVertex = normalize(v_uv - u_cameraPos);
    float specComp = pow(max(dot(reflectDir, -cameraToVertex), 0.0), 128.0);
    vec4 specular = vec4(light.specular, 1.0) * t_spec * specComp * attenuation;

    return (ambient + diffuse + specular);
}

void main()
{
    vec4 t = texture(u_texture, v_textureUv);
    if (t.a == 0.0)
        discard;

    resultColor = getPointLightComponent(u_pointLight, t, t);
}