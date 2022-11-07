#include "Sandbox/blend_scene.h"

#include "Common/application.h"

BlendScene::BlendScene(GLFWwindow* context) : Scene(context, "Blending Scene")
    , m_camera(context)
    , m_texture("../Data/Texture/grass.png", TextureType::DIFFUSE, false, GL_CLAMP_TO_EDGE)
    , m_pointLight(glm::vec3(1.0f, 0.09f, 0.03f)
        , glm::vec3(4.0f, 0.0f, 2.0f)
        , glm::vec3(0.3f, 0.28f, 0.65f)
        , glm::vec3(1.0f, 0.25f, 0.2f)
        , glm::vec3(1.0f, 0.85f, 1.0f))
{
    Application::GetInstance()->GetWindow().AddCursorCallback([&](Window* window, double x, double y) 
    { 
        m_camera.UpdateEulers(x, y, window->IsCursorVisible() || !window->IsFocused()); 
    });
    Application::GetInstance()->GetWindow().AddScrollCallback([&](Window* window, double y) { m_camera.UpdateScroll((float) y); });
    m_blendShader = CreateShader("../Data/Shader/blend.vert", "../Data/Shader/blend.frag");
    
    std::array<float, 8 * 6> vertices = {
        // positions          // Normals         // texture Coords (swapped y coordinates because texture is flipped upside down)
        -0.5f,  0.5f,  0.0f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.0f,  0.0f, 0.0f, 1.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.0f,  0.0f, 0.0f, 1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.0f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.0f,  0.0f, 0.0f, 1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.0f,  0.0f, 0.0f, 1.0f,  1.0f,  0.0f
    };

    VertexArray::Bind(m_vao);

    VertexBuffer::Bind(m_vbo);
    VertexBuffer::Data(vertices.begin(), vertices.end(), GL_STATIC_DRAW);

    VertexArray::AddLayout(m_vao, {
        VBLayout{3, GL_FLOAT},
        VBLayout{3, GL_FLOAT},
        VBLayout{2, GL_FLOAT}
    });
}

void BlendScene::OnAttach()
{
    // http://andersriggelsen.dk/glblendfunc.php
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);
}

void BlendScene::OnDetatch()
{
    glDisable(GL_BLEND);
}

void BlendScene::Update(float timeStep)
{
    Window& window = Application::GetInstance()->GetWindow();
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_translation);
    model = glm::scale(model, m_scale);
    model = glm::rotate(model, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    m_camera.UpdatePosition(timeStep);

    int32_t width = window.GetWidth();
    int32_t height = window.GetHeight();
    glm::mat4 projection = glm::perspective(glm::radians(m_camera.Fov()), (float)width/(float)height, 0.1f, 100.0f);

    Shader::Bind(*m_blendShader);
    Texture2D::Bind(m_texture, 0);
    m_blendShader->SetUniformMat4("u_model", model);
    m_blendShader->SetUniformMat4("u_projection", projection);
    m_blendShader->SetUniformMat4("u_view", m_camera.GetLookAtMatrix());
    m_blendShader->SetUniformPointLight("u_pointLight", m_pointLight);
    m_blendShader->SetUniformVec3("u_cameraPos", m_camera.Position());
    m_blendShader->SetUniform1i("u_texture", 0);

    VertexArray::Bind(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    m_blendShader->SetUniformMat4("u_model", model);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    VertexArray::Unbind();
    Texture2D::Unbind();
}

void BlendScene::UpdateImgui(ImGuiIO& io, float timeStep)
{
    ImGui::Begin("Blending Scene tests!");

    ImGui::Text("Model");
    ImGui::SliderFloat3("Transl", glm::value_ptr(m_translation), -10.0f, 10.0f);
    ImGui::SliderFloat3("Rotation", glm::value_ptr(m_rotation), -180.0f, 180.0f);
    ImGui::SliderFloat3("Scale", glm::value_ptr(m_scale), 0.1f, 10.0f);

    ImGui::Text("Lights");
    ImGui::SliderFloat3("posi", glm::value_ptr(m_pointLight.pos), -3.0f, 3.0f);
    ImGui::SliderFloat3("ambi", glm::value_ptr(m_pointLight.ambient), 0.0f, 1.0f);
    ImGui::SliderFloat3("diff", glm::value_ptr(m_pointLight.diffuse), 0.0f, 1.0f);
    ImGui::SliderFloat3("spec", glm::value_ptr(m_pointLight.specular), 0.0f, 1.0f);

    ImGui::End();
}