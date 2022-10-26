#include "application.h"
#include "scene.h"

class PlantScene : public Scene
{
private:
    Model* m_plant;
    Shader* m_shader;

    glm::vec3 m_translation = glm::vec3(0.0f);
    glm::vec3 m_scale = glm::vec3(1.0f);
    glm::vec3 m_rotation = glm::vec3(0.0f);

public:
    PlantScene(GLFWwindow* context, Camera& camera) : Scene(context, camera) 
    {
        m_plant = createModel("../Data/Model/survival_backpack/backpack.obj", true);
        m_shader = createShader("../Data/Shader/texture_default.vert", "../Data/Shader/texture_default.frag");
    }

    virtual void update(float timeStep) override
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, m_scale);
        model = glm::translate(model, m_translation);

        int32_t width;
        int32_t height;
        glfwGetWindowSize(m_context, &width, &height);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f);
        glm::mat4 view = m_camera.constructLookAt();

        m_shader->bind();
        m_shader->setUniformMat4("u_model", model);
        m_shader->setUniformMat4("u_projection", projection);
        m_shader->setUniformMat4("u_view", view);
        m_plant->render(*m_shader, "u_material");
    }
};

int main() 
{
    std::unique_ptr<Application> application = std::make_unique<Application>(1920, 1080, "3D Renderer");
    application->getWindow().getRenderManager().addScene<PlantScene>(
        application->getWindow().getContext()
        );
    application->run();
}
