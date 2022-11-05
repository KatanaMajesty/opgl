#include "model_scene.h"

#include "Common/window.h"

ModelScene::ModelScene(GLFWwindow* context) : Scene(context)
    , m_camera(context)
{
    Window::GetInstance()->AddCursorCallback([&](Window* window, double x, double y) { m_camera.UpdateEulers(x, y, window->IsCursorVisible() || !window->IsFocused()); });
    Window::GetInstance()->AddScrollCallback([&](Window* window, double y) { m_camera.UpdateScroll((float) y); });
    Window::GetInstance()->AddKeyCallback(GLFW_KEY_F1, [](Window* window, float timeStep, int32_t action)
    {
        if (action == GLFW_PRESS)
            window->SetCursorVisible(!window->IsCursorVisible());
    });
    Window::GetInstance()->AddKeyCallback(GLFW_KEY_ESCAPE, [](Window* window, float timeStep, int32_t action) { window->Close(); });

    AddModel("backpack", CreateModel("../Data/Model/survival_backpack/backpack.obj", true, true));
    m_shader = CreateShader("../Data/Shader/texture_default.vert", "../Data/Shader/texture_default.frag");
    m_stencilShader = CreateShader("../Data/Shader/depth_shader.vert", "../Data/Shader/outline.frag");
    m_depthShader = CreateShader("../Data/Shader/depth_shader.vert", "../Data/Shader/depth_shader.frag");

    glDepthFunc(GL_LESS);
}

void ModelScene::Update(float timeStep)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, m_scale);
    model = glm::translate(model, m_translation);

    Model* ptr = m_models.at("backpack");

    m_camera.UpdatePosition(timeStep);

    int32_t width = Window::GetInstance()->GetWidth();
    int32_t height = Window::GetInstance()->GetHeight();
    glm::mat4 projection = glm::perspective(glm::radians(m_camera.Fov()), (float)width/(float)height, 0.1f, 100.0f);

#if 0
    Shader::Bind(*m_depthShader);
    m_depthShader->SetUniformMat4("u_model", model);
    m_depthShader->SetUniformMat4("u_projection", projection);
    m_depthShader->SetUniformMat4("u_view", m_camera.GetLookAtMatrix());
    // m_crate->Render(*m_depthShader, "u_material");
    // model = glm::translate(model, glm::vec3(0.0f, 0.0f, 5.0f));
    // m_depthShader->SetUniformMat4("u_model", model);
    // m_box->Render(*m_depthShader, "u_material");
#else
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF); // enable writing to & bitmask
    
    Shader::Bind(*m_shader);
    m_shader->SetUniformMat4("u_model", model);
    m_shader->SetUniformMat4("u_projection", projection);
    m_shader->SetUniformMat4("u_view", m_camera.GetLookAtMatrix());
    m_shader->SetUniform1i("u_enableLighting", m_enableLights);
    m_shader->SetUniformVec3("u_cameraPos", m_camera.Position());
    m_shader->SetUniformDirLight("u_dirLight", m_dirLight);
    ptr->Render(*m_shader, "u_material");

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00); // disable writing to & bitmask
    glDisable(GL_DEPTH_TEST);

    Shader::Bind(*m_stencilShader);
    model = glm::scale(model, glm::vec3(1.01f));
    m_stencilShader->SetUniformMat4("u_model", model);
    m_stencilShader->SetUniformMat4("u_projection", projection);
    m_stencilShader->SetUniformMat4("u_view", m_camera.GetLookAtMatrix());
    ptr->Render(*m_stencilShader, "u_material");
    
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF); // enable writing to & bitmask
    glEnable(GL_DEPTH_TEST);
#endif
}

void ModelScene::UpdateImgui(ImGuiIO& io, float timeStep)
{
    ImGui::Begin("Interaction");

    float fps = io.Framerate;
    ImGui::TextColored(ImVec4(60.0f / fps, fps / 600.0f, 0.2f, 1.0f), "Current FPS: %f", fps);

    ImGui::Text("Model");
    ImGui::SliderFloat3("Transl", glm::value_ptr(m_translation), -10.0f, 10.0f);
    ImGui::SliderFloat3("Rotation", glm::value_ptr(m_rotation), -180.0f, 180.0f);
    ImGui::SliderFloat3("Scale", glm::value_ptr(m_scale), 0.1f, 10.0f);

    ImGui::Text("Lights");    
    ImGui::SliderFloat3("dire", glm::value_ptr(m_dirLight.dir), -3.0f, 3.0f);
    ImGui::SliderFloat3("ambi", glm::value_ptr(m_dirLight.ambient), 0.0f, 1.0f);
    ImGui::SliderFloat3("diff", glm::value_ptr(m_dirLight.diffuse), 0.0f, 1.0f);
    ImGui::SliderFloat3("spec", glm::value_ptr(m_dirLight.specular), 0.0f, 1.0f);
    ImGui::Checkbox("Enable Lights", &m_enableLights);

    ImGui::End();
}