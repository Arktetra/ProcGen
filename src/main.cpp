#include <iostream>
#include <vector>

#include "./includes/glm/gtc/matrix_transform.hpp"

#include "platform/window.hpp"
#include "platform/shader.hpp"
#include "platform/buffer.hpp"
#include "platform/texture.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

glm::vec3 camera_pos = glm::vec3(0.0, 0.0, 3.0);    // a vector in world space that points to the camera's position
glm::vec3 camera_front = glm::vec3(0.0, 0.0, -1.0); // camera direction vector
glm::vec3 camera_up = glm::vec3(0.0, 1.0, 0.0);     //  camera's positive y-axis

bool first_mouse = true;
float yaw = -90.0;
float pitch = 0.0;
float last_x = 800.0 / 2.0;
float last_y = 600.0 / 2.0;
float fov = 45.0f;

float delta_time = 0.0f;
float last_frame = 0.0f;

int main() {
    Window window = Window();

    glfwSetFramebufferSizeCallback(window.ptr, framebuffer_size_callback);

    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMakeContextCurrent(window.ptr);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Make sure GLAD is initialized.
    Debug::init();

    Shader shader("./src/shaders/triangle.vert", "./src/shaders/triangle.frag");

    std::vector<float> vertices = {
        // positions         // colors
        2.0f, -2.0f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f,   // bottom right
        -2.0f, -2.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  // bottom left
        2.0f,  2.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,   // top 
        -2.0f, 2.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f
    };  

    std::vector<int> indices {
        0, 1, 2,
        1, 2, 3
    };

    Buffer buffer(vertices, indices, GL_STATIC_DRAW);
    ProcGen::config_vertex_attribute();
    Texture tex("./assets/imgs/clouds.jpg");

    while (!glfwWindowShouldClose(window.ptr)) {
        process_input(window.ptr);

        float current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        glClearColor(0.2, 0.3, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        tex.bind();
        shader.use();

        // create transformations
        glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        // view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        view = glm::lookAt(camera_pos, camera_front + camera_pos, camera_up);
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        
        shader.set_mat4("model", model);
        shader.set_mat4("view", view);
        shader.set_mat4("projection", projection);

        buffer.bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window.ptr);
        glfwPollEvents();
    }
    shader.remove();

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    float camera_speed = static_cast<float>(2.5 * delta_time);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera_pos += camera_speed * camera_front;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera_pos -= camera_speed * camera_front;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
    }
}