#pragma once

#include "lighting.h"

struct Material
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

class Shader
{
private:
    GLuint m_program;
    // uniform cache uses sort of doulbe-hashing system.
    // Before adding uniform location to cache, we hash the std::string, so that there is no UB with memory
    std::unordered_map<std::string, GLuint> m_uniformCache;

public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);  
    
    ~Shader();

    inline void bind() const { glUseProgram(m_program); }
    
    void setUniform1i(const std::string& uniform, int32_t i);
    void setUniform1f(const std::string& uniform, float f);
    void setUniformVec4(const std::string& uniform, const glm::vec4& v);
    void setUniformVec3(const std::string& uniform, const glm::vec3& v);
    void setUniformMat4(const std::string& uniform, const glm::mat4& m);
    void setUniformSpotLight(const std::string& uniform, const SpotLight& light);
    void setUniformPointLight(const std::string& uniform, const PointLight& light);
    void setUniformDirLight(const std::string& uniform, const DirectionalLight& light);

    GLint getUniformLocation(const std::string& uniform);
};
