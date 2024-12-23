#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <STB/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"


float vertices[] = {
   -0.5f,  0.0f,  0.5f,     0.5f, 0.0f, 0.5f,   0.0f, 0.0f, 
   -0.5f,  0.0f, -0.5f,     0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 
    0.5f,  0.0f, -0.5f,     0.0f, 0.5f, 0.5f,   0.0f, 0.0f,
    0.5f,  0.0f,  0.5f,     1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 
    0.0f,  0.8f,  0.0f,     0.0f, 0.5f, 0.5f,   0.5f, 1.0f, 

};

unsigned int indices[] = {
    0, 1, 2,
    0, 2, 3,
    0, 1, 4,
    1, 2, 4,
    2, 3, 4,
    3, 0, 4
};

const unsigned int width = 800;
const unsigned int height = 600;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor; \n"
"layout (location = 2) in vec2 aTex;\n"
"out vec3 ourColor;\n"
"out vec2 texCoord;\n"
"uniform mat4 CameraMatrix;\n"
"void main()\n"
"{\n"
"    gl_Position = CameraMatrix * vec4(aPos, 1.0);\n"
"    ourColor = aColor;\n"
"    texCoord = aTex;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"in vec2 texCoord;\n"
"uniform sampler2D tex0;\n"
"void main()\n"
"{\n"
"    FragColor = texture(tex0, texCoord);\n"
"}\n\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE == GLFW_PRESS))
        glfwSetWindowShouldClose(window, true);
}

int main(void) {

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Computacion Grafica", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Error al crear la ventana" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //Inicializar GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Error al inicializar GLAD" << std::endl;
        return -1;
    }

    //vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    //fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    //link shader
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    unsigned int VBO, VAO, IBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    unsigned int stride = 8 * sizeof(float);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    //Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    int widhtImg, heightImg, PixelNum;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load("chill.jpg", &widhtImg, &heightImg, &PixelNum, 0);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (bytes) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widhtImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);

    //UNIFORMS
    GLuint texUniform = glGetUniformLocation(shaderProgram, "tex0");
    glUseProgram(shaderProgram);
    glUniform1i(texUniform, 0);

    glEnable(GL_DEPTH_TEST);

    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

    // Bucle principal
    while (!glfwWindowShouldClose(window)) {

        processInput(window);

        //INSERT RENDERING COMMANDS
        glClearColor(0.5f, 0.7f, 1.0f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        camera.CameraMatrix(45.0f, 0.1f, 100.0f, shaderProgram, "CameraMatrix");
        camera.CameraInputs(window);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 5);
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
    glfwTerminate();
    return 0;
}