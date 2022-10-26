#pragma once

class DirectionalLight
{
public:
    glm::vec3 dir;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    DirectionalLight(glm::vec3 dir, glm::vec3 ambient, glm::vec3 diff, glm::vec3 spec);
};

// https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
class PointLight
{
public:
    // constant, linear and quadratic terms of attenuation
    // constant should usually stay at 1, so there is no brightness boost within bigger distances
    glm::vec3 clq;
    glm::vec3 pos;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    PointLight() = default;
    PointLight(glm::vec3 clq, glm::vec3 pos, glm::vec3 ambient, glm::vec3 diff, glm::vec3 spec);
};

class SpotLight
{
public:
    float cutoff;
    float outerCutoff;
    glm::vec3 clq;
    glm::vec3 pos;
    glm::vec3 dir;
    glm::vec3 diffuse;
    glm::vec3 specular;

    SpotLight(glm::vec3 clq, glm::vec3 pos, glm::vec3 dir, glm::vec3 diff, glm::vec3 spec, float cutoff = 12.5f, float outerCutoff = 14.5f);
};