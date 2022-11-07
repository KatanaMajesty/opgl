#pragma once

#include "Graphics/scene.h"
#include "Graphics/lighting.h"
#include "Common/camera.h"

#include "OpenGL/vertex_array.h"
#include "OpenGL/vertex_buffer.h"
#include "OpenGL/index_buffer.h"
#include "OpenGL/texture2d.h"

class BlendScene : public Scene
{
private:
    glm::mat4 m_model;
    glm::mat4 m_view;
    glm::mat4 m_projection;
    Camera m_camera;
    Shader* m_blendShader;
    VertexArray m_vao;
    VertexBuffer m_vbo;
    IndexBuffer m_ibo;
    Texture2D m_texture;
    glm::vec3 m_translation = glm::vec3(0.0f);
    glm::vec3 m_scale = glm::vec3(1.0f);
    glm::vec3 m_rotation = glm::vec3(0.0f);
    PointLight m_pointLight;

public:
    BlendScene(GLFWwindow* context);

    virtual void OnAttach() override;
    virtual void OnDetatch() override;

    virtual void Update(float timeStep) override;
    virtual void UpdateImgui(ImGuiIO& io, float timeStep) override;
};