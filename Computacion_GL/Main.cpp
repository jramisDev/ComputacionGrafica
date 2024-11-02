#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void frameBuffer_size_callback(GLFWwindow* Window, int width, int height) {

    glViewport(0, 0, width, height);

}

int main(void)
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* Window = glfwCreateWindow(800,600, "Computacion Grafica", nullptr, nullptr);

    if (Window == nullptr) {
        std::cout << "Failed to create the Window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(Window);

    //GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to inizialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(Window, frameBuffer_size_callback);

    while (!glfwWindowShouldClose(Window)) {

        glfwSwapBuffers(Window);
        glfwPollEvents();

    }

    glViewport(0, 0, 800, 600);

    glfwTerminate();
    return 0;
}