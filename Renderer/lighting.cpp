#include "lighting.h"


DirectionalLight::DirectionalLight(glm::vec3 dir, glm::vec3 ambient, glm::vec3 diff, glm::vec3 spec)
    : dir(dir)
    , ambient(ambient)
    , diffuse(diff)
    , specular(spec)    
{
}

PointLight::PointLight(glm::vec3 clq, glm::vec3 pos, glm::vec3 ambient, glm::vec3 diff, glm::vec3 spec)
    : clq(clq)
    , pos(pos)
    , ambient(ambient)
    , diffuse(diff)
    , specular(spec)    
{
}

SpotLight::SpotLight(glm::vec3 clq, glm::vec3 pos, glm::vec3 dir, glm::vec3 diff, glm::vec3 spec, float cutoff, float outerCutoff)
    : cutoff(cutoff), outerCutoff(outerCutoff)
    , clq(clq)
    , pos(pos)
    , dir(dir)
    , diffuse(diff)
    , specular(spec)    
{
}