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
    // std::unordered_map<std::string, GLuint> m_uniformCache;

public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);  
    
    ~Shader();

    inline void bind() const { glUseProgram(m_program); }
    
    void setUniform1i(const char* uniform, int32_t i);
	void setUniform1f(const char* uniform, float f);
    void setUniformVec4(const char* uniform, const glm::vec4& v);
    void setUniformVec3(const char* uniform, const glm::vec3& v);
	void setUniformMat4(const char* uniform, const glm::mat4& m);
    void setUniformSpotLight(const char* uniform, const SpotLight& light);
    void setUniformPointLight(const char* uniform, const PointLight& light);
    void setUniformDirLight(const char* uniform, const DirectionalLight& light);

    GLint getUniformLocation(const char* uniform);
};
