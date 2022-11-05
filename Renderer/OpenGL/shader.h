#pragma once

#include "Graphics/lighting.h"

struct Material
{
    glm::vec3 ambient = glm::vec3(0.0f);
    glm::vec3 diffuse = glm::vec3(0.0f);
    glm::vec3 specular = glm::vec3(0.0f);
    float shininess = 0.0f;
};

class Shader
{
private:
    GLuint m_program;
    std::unordered_map<std::string, GLuint> m_uniformCache;

public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);  
    ~Shader();

    static inline void Bind(Shader& shader) { glUseProgram(shader.m_program); }

    static inline void Unbind() { glUseProgram(0); }
    
    void SetUniform1i(const std::string& uniform, int32_t i);

    void SetUniform1f(const std::string& uniform, float f);

    void SetUniformVec4(const std::string& uniform, const glm::vec4& v);
    
    void SetUniformVec3(const std::string& uniform, const glm::vec3& v);
    
    void SetUniformMat4(const std::string& uniform, const glm::mat4& m);
    
    void SetUniformSpotLight(const std::string& uniform, const SpotLight& light);
    
    void SetUniformPointLight(const std::string& uniform, const PointLight& light);
    
    void SetUniformDirLight(const std::string& uniform, const DirectionalLight& light);

    void SetUniformMaterial(const std::string& uniform, const Material& material);

private:
    GLint GetUniformLocation(const std::string& uniform);
};
