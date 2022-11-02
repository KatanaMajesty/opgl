#include "application.h"
#include "scene.h"
#include "lighting.h"

class PlantScene : public Scene
{
private:
    Model* m_plant;
    Model* m_backpack;
    Shader* m_shader;

    Camera m_camera;

    glm::vec3 m_translation = glm::vec3(0.0f);
    glm::vec3 m_scale = glm::vec3(1.0f);
    glm::vec3 m_rotation = glm::vec3(0.0f);

    DirectionalLight m_dirLight = {
        glm::vec3(-1.0f, -2.0f, -3.0f),
        glm::vec3(0.255f, 0.14f, 0.1f),
        glm::vec3(0.14f, 0.1f, 0.09f),
        glm::vec3(0.255f, 0.15f, 0.1f),
    };

    bool m_enableLights = true;

public:
    PlantScene(GLFWwindow* context) : Scene(context)
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

        m_plant = CreateModel("../Data/Model/silver_splash/Silver_Splash_Pothos_SF.obj");
        m_backpack = CreateModel("../Data/Model/survival_backpack/backpack.obj", true);
        m_shader = CreateShader("../Data/Shader/texture_default.vert", "../Data/Shader/texture_default.frag");
    }

    virtual void Update(float timeStep) override
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, m_scale);
        model = glm::translate(model, m_translation);

        m_camera.UpdatePosition(timeStep);

        int32_t width = Window::GetInstance()->GetWidth();
        int32_t height = Window::GetInstance()->GetHeight();
        glm::mat4 projection = glm::perspective(glm::radians(m_camera.Fov()), (float)width/(float)height, 0.1f, 100.0f);

        m_shader->bind();
        m_shader->setUniformMat4("u_model", model);
        m_shader->setUniformMat4("u_projection", projection);
        m_shader->setUniformMat4("u_view", m_camera.GetLookAtMatrix());
        m_shader->setUniform1i("u_enableLighting", m_enableLights);
        m_shader->setUniformVec3("u_cameraPos", m_camera.Position());
        m_shader->setUniformDirLight("u_dirLight", m_dirLight);
        m_plant->Render(*m_shader, "u_material");
        model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f));
        m_shader->setUniformMat4("u_model", model);
        m_backpack->Render(*m_shader, "u_material");
    }

    virtual void UpdateImgui(ImGuiIO& io, float timeStep) override
    {
        ImGui::Begin("Interaction");

        float fps = io.Framerate;
        ImGui::TextColored(ImVec4(60.0f / fps, fps / 600.0f, 0.2f, 1.0f), "Current FPS: %f", fps);
        ImGui::SliderFloat3("dire", glm::value_ptr(m_dirLight.dir), -3.0f, 3.0f);
        ImGui::SliderFloat3("ambi", glm::value_ptr(m_dirLight.ambient), 0.0f, 1.0f);
        ImGui::SliderFloat3("diff", glm::value_ptr(m_dirLight.diffuse), 0.0f, 1.0f);
        ImGui::SliderFloat3("spec", glm::value_ptr(m_dirLight.specular), 0.0f, 1.0f);
        ImGui::Checkbox("Enable Lights", &m_enableLights);

        ImGui::End();
    }
};

int main() 
{
    std::unique_ptr<Application> application = std::make_unique<Application>();
    Window::GetInstance()->AddScene<PlantScene>();
    application->Run();
}
