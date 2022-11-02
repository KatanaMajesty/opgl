#include "shader.h"

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
    : m_program(glCreateProgram())
{
    std::ifstream ifs;
    // Stores 2 pairs <shaderPath, shaderId>
    std::array<std::pair<std::string_view, GLuint>, 2> shaderPaths;
    shaderPaths.at(0) = { std::string_view{vertexPath}, -1 };
    shaderPaths.at(1) = { std::string_view{fragmentPath}, -1 };

    for (size_t i = 0; i < shaderPaths.size(); i++)
    {
        std::string_view v = shaderPaths[i].first;
        ifs.open(v.data());
        if (!ifs.is_open())
        {
            m_program = -1;
            std::cout << "Failed to parse shader: " << v << std::endl;
            break;
        }
        std::string src = std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
        ifs.close();
        const char* sc = src.c_str();

        GLuint& shaderId = shaderPaths[i].second;
        shaderId = glCreateShader(i == 0 ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
        glShaderSource(shaderId, 1, &sc, nullptr);
        glCompileShader(shaderId);
    
        int success;
        char infoLog[512];
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        glAttachShader(m_program, shaderId);
    }
    glLinkProgram(m_program);
    for (auto& pair : shaderPaths)
    {
        glDeleteShader(pair.second);
    }
}

Shader::~Shader()
{
    glDeleteProgram(m_program);
}

void Shader::setUniform1i(const std::string& uniform, int32_t i)
{
    GLint loc = getUniformLocation(uniform);
    glUniform1i(loc, i);
}

void Shader::setUniform1f(const std::string& uniform, float f)
{
	GLint loc = getUniformLocation(uniform);
	glUniform1f(loc, f);
}

void Shader::setUniformVec4(const std::string& uniform, const glm::vec4& v)
{
    GLint loc = getUniformLocation(uniform);
    glUniform4fv(loc, 1, glm::value_ptr(v));
}

void Shader::setUniformVec3(const std::string& uniform, const glm::vec3& v)
{
    GLint loc = getUniformLocation(uniform);
    glUniform3fv(loc, 1, glm::value_ptr(v));
}

void Shader::setUniformMat4(const std::string& uniform, const glm::mat4& m)
{
	GLint loc = getUniformLocation(uniform);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
}

void Shader::setUniformSpotLight(const std::string& uniform, const SpotLight& light)
{
    std::string s(uniform);
    setUniform1f((s + ".cutoff").c_str(), glm::cos(glm::radians(light.cutoff)));
    setUniform1f((s + ".outerCutoff").c_str(), glm::cos(glm::radians(light.outerCutoff)));
    setUniformVec3((s + ".clq").c_str(), light.clq);
    setUniformVec3((s + ".pos").c_str(), light.pos);
    setUniformVec3((s + ".dir").c_str(), light.dir);
    setUniformVec3((s + ".diffuse").c_str(), light.diffuse);
    setUniformVec3((s + ".specular").c_str(), light.specular);
}
void Shader::setUniformPointLight(const std::string& uniform, const PointLight& light)
{
    std::string s(uniform);
    setUniformVec3((s + ".clq").c_str(), light.clq);
    setUniformVec3((s + ".pos").c_str(), light.pos);
    setUniformVec3((s + ".ambient").c_str(), light.ambient);
    setUniformVec3((s + ".diffuse").c_str(), light.diffuse);
    setUniformVec3((s + ".specular").c_str(), light.specular);
}

void Shader::setUniformDirLight(const std::string& uniform, const DirectionalLight& light)
{
    std::string s(uniform);
    setUniformVec3((s + ".dir").c_str(), light.dir);
    setUniformVec3((s + ".ambient").c_str(), light.ambient);
    setUniformVec3((s + ".diffuse").c_str(), light.diffuse);
    setUniformVec3((s + ".specular").c_str(), light.specular);
}

GLint Shader::getUniformLocation(const std::string& uniform)
{
    auto it = m_uniformCache.find(uniform);
    GLint loc = it == m_uniformCache.end() ? glGetUniformLocation(m_program, uniform.c_str()) : it->second;
    m_uniformCache.emplace(uniform, loc);
    return loc;
} 
