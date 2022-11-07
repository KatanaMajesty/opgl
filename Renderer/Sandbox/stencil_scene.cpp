#include "Sandbox/stencil_scene.h"

#include "Common/application.h"

StencilScene::StencilScene(GLFWwindow* context) : Scene(context, "Stencil Test Scene")
    , m_camera(context)
{
    m_defaultShader = CreateShader("../Data/Shader/texture_default.vert", "../Data/Shader/texture_default.frag");
    m_stencilShader = CreateShader("../Data/Shader/outline.vert", "../Data/Shader/outline.frag");
    m_chair = CreateModel("../Data/Model/chair/scene.gltf", true, false);
    Application::GetInstance()->GetWindow().AddCursorCallback([&](Window* window, double x, double y)
    {
        m_camera.UpdateEulers(x, y, window->IsCursorVisible() || !window->IsFocused());
    });

    Application::GetInstance()->GetWindow().AddScrollCallback([&](Window* window, double y)
    {
        m_camera.UpdateScroll(static_cast<float>(y));
    });
}

void StencilScene::OnAttach()
{
    glEnable(GL_STENCIL_TEST);
    Scene::OnAttach();
}

void StencilScene::OnDetatch()
{
    glDisable(GL_STENCIL_TEST);
    Scene::OnDetatch();
}

void StencilScene::Update(float timeStep)
{
    Window& window = Application::GetInstance()->GetWindow();

    glm::mat4 projection = glm::perspective(glm::radians(m_camera.Fov()), (float)window.GetWidth()/(float)window.GetHeight(), 0.1f, 1000.0f);
    
    m_camera.UpdatePosition(timeStep);
    glm::mat4 view = m_camera.GetLookAtMatrix();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_translation);
    model = glm::scale(model, m_scale);
    model = glm::rotate(model, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    Model* ptr = m_chair; // for easy model changing

    glStencilMask(0xFF); // enable writing
    glStencilFunc(GL_ALWAYS, 1, 0xFF); // always write 1 to stenctil buff
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    Shader::Bind(*m_defaultShader);
    m_defaultShader->SetUniformMat4("u_model", model);
    m_defaultShader->SetUniformMat4("u_view", view);
    m_defaultShader->SetUniformMat4("u_projection", projection);
    m_defaultShader->SetUniformDirLight("u_dirLight", m_dirLight);
    m_defaultShader->SetUniformVec3("u_cameraPos", m_camera.Position());
    m_defaultShader->SetUniform1i("u_enableLighting", true);
    ptr->Render(*m_defaultShader, "u_material");

    if (m_selection)
    {
        glStencilMask(0x00);
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // if not equal to 1 than draw
        glDisable(GL_DEPTH_TEST); // so that we can see selection through other objects

        Shader::Bind(*m_stencilShader);
        model = glm::scale(model, glm::vec3(1.01f));
        m_stencilShader->SetUniformMat4("u_model", model);
        m_stencilShader->SetUniformMat4("u_view", view);
        m_stencilShader->SetUniformMat4("u_projection", projection);
        m_stencilShader->SetUniform1f("u_outlineFactor", 0.01f);
        ptr->Render(*m_stencilShader, "u_material");

        glEnable(GL_DEPTH_TEST);
        glStencilMask(0xFF); // enable writing
        glStencilFunc(GL_ALWAYS, 1, 0xFF); // always write 1 to stenctil buff
    }
}

void StencilScene::UpdateImgui(ImGuiIO& io, float timeStep)
{
    ImGui::Begin("Interaction");

    float fps = io.Framerate;
    ImGui::TextColored(ImVec4(60.0f / fps, fps / 600.0f, 0.2f, 1.0f), "Current FPS: %f", fps);
    ImGui::Checkbox("Toggle Selection", &m_selection);

    ImGui::Text("Lights");
    ImGui::SliderFloat3("dire", glm::value_ptr(m_dirLight.dir), -3.0f, 3.0f);
    ImGui::SliderFloat3("ambi", glm::value_ptr(m_dirLight.ambient), 0.0f, 1.0f);
    ImGui::SliderFloat3("diff", glm::value_ptr(m_dirLight.diffuse), 0.0f, 1.0f);
    ImGui::SliderFloat3("spec", glm::value_ptr(m_dirLight.specular), 0.0f, 1.0f);
    
    ImGui::End();
}