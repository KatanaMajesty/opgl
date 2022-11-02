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

void Shader::SetUniform1i(const std::string& uniform, int32_t i)
{
    GLint loc = GetUniformLocation(uniform);
    glUniform1i(loc, i);
}

void Shader::SetUniform1f(const std::string& uniform, float f)
{
	GLint loc = GetUniformLocation(uniform);
	glUniform1f(loc, f);
}

void Shader::SetUniformVec4(const std::string& uniform, const glm::vec4& v)
{
    GLint loc = GetUniformLocation(uniform);
    glUniform4fv(loc, 1, glm::value_ptr(v));
}

void Shader::SetUniformVec3(const std::string& uniform, const glm::vec3& v)
{
    GLint loc = GetUniformLocation(uniform);
    glUniform3fv(loc, 1, glm::value_ptr(v));
}

void Shader::SetUniformMat4(const std::string& uniform, const glm::mat4& m)
{
	GLint loc = GetUniformLocation(uniform);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
}

void Shader::SetUniformSpotLight(const std::string& uniform, const SpotLight& light)
{
    SetUniform1f(uniform + ".cutoff", glm::cos(glm::radians(light.cutoff)));
    SetUniform1f(uniform + ".outerCutoff", glm::cos(glm::radians(light.outerCutoff)));
    SetUniformVec3(uniform + ".clq", light.clq);
    SetUniformVec3(uniform + ".pos", light.pos);
    SetUniformVec3(uniform + ".dir", light.dir);
    SetUniformVec3(uniform + ".diffuse", light.diffuse);
    SetUniformVec3(uniform + ".specular", light.specular);
}
void Shader::SetUniformPointLight(const std::string& uniform, const PointLight& light)
{
    SetUniformVec3(uniform + ".clq", light.clq);
    SetUniformVec3(uniform + ".pos", light.pos);
    SetUniformVec3(uniform + ".ambient", light.ambient);
    SetUniformVec3(uniform + ".diffuse", light.diffuse);
    SetUniformVec3(uniform + ".specular", light.specular);
}

void Shader::SetUniformDirLight(const std::string& uniform, const DirectionalLight& light)
{
    SetUniformVec3(uniform + ".dir", light.dir);
    SetUniformVec3(uniform + ".ambient", light.ambient);
    SetUniformVec3(uniform + ".diffuse", light.diffuse);
    SetUniformVec3(uniform + ".specular", light.specular);
}

void Shader::SetUniformMaterial(const std::string& uniform, const Material& material)
{
    SetUniformVec3(uniform + ".ambient", material.ambient);
    SetUniformVec3(uniform + ".diffuse", material.diffuse);
    SetUniformVec3(uniform + ".specular", material.specular);
    SetUniform1f(uniform + ".shininess", material.shininess);
}

GLint Shader::GetUniformLocation(const std::string& uniform)
{
    Shader::Bind(*this); // just to ensure, we get uniform location in correct shader
    auto it = m_uniformCache.find(uniform);
    GLint loc = it == m_uniformCache.end() ? glGetUniformLocation(m_program, uniform.c_str()) : it->second;
    m_uniformCache.emplace(uniform, loc);
    return loc;
} 
