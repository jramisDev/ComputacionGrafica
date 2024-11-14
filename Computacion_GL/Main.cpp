#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shaders/Shader.h"
#include "ShadersProgram/ShaderProgram.h"

// C�digo fuente de los shaders
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 alternativeColor;\n"
"void main()\n"
"{\n"
"   FragColor = alternativeColor;\n"
"}\n\0";

float vertices[] = {
     0.0f,  0.0f, 0.0f,
     0.5f,  0.0f, 0.0f,
     0.0f,  0.5f, 0.0f,
    -0.5f,  0.0f, 0.0f,
     0.0f, -0.5f, 0.0f
};

unsigned int indices[] = {
    0,1,2,
    0,2,3,
    0,3,4,
    0,4,1
};

void frameBuffer_size_callback(GLFWwindow* Window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main(void)
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* Window = glfwCreateWindow(800, 600, "Computacion Grafica", nullptr, nullptr);

    if (Window == nullptr) {
        std::cerr << "Failed to create the Window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(Window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(Window, frameBuffer_size_callback);

    Shader vertexShader(vertexShaderSource, GL_VERTEX_SHADER);
    Shader fragmentShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

    ShaderProgram shaderProgram;
    shaderProgram.attachShader(vertexShader);
    shaderProgram.attachShader(fragmentShader);
    shaderProgram.link();

    int alternativeColorLocation = shaderProgram.getUniformLocation("alternativeColor");

    unsigned int VBO, VAO, IBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Wireframe activado
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(Window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.use();
        glBindVertexArray(VAO);

        glUniform4f(alternativeColorLocation, 1.f, 1.f, 0.f, 1.f);
        glDrawElements(GL_TRIANGLES, std::size(indices) / 2, GL_UNSIGNED_INT, 0);

        glUniform4f(alternativeColorLocation, 0.0f, 1.f, 0.f, 1.f);
        glDrawElements(GL_TRIANGLES, std::size(indices) / 2, GL_UNSIGNED_INT, (void*)(3 * sizeof(unsigned int)));

        glUniform4f(alternativeColorLocation, 0.0f, 0.f, 1.f, 1.f);
        glDrawElements(GL_TRIANGLES, std::size(indices) / 2, GL_UNSIGNED_INT, (void*)(6 * sizeof(unsigned int)));

        glUniform4f(alternativeColorLocation, 1.0f, 0.f, 0.f, 1.f);
        glDrawElements(GL_TRIANGLES, std::size(indices) / 2, GL_UNSIGNED_INT, (void*)(9 * sizeof(unsigned int)));

        glfwSwapBuffers(Window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);

    glfwTerminate();
    return 0;
}
