#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <STB/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "Mesh.h"  


std::vector<float> pyramidVertices = {
   -0.5f,  0.0f,  0.5f,     0.5f, 0.0f, 0.5f,   0.0f, 0.0f,
   -0.5f,  0.0f, -0.5f,     0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
    0.5f,  0.0f, -0.5f,     0.0f, 0.5f, 0.5f,   0.0f, 0.0f,
    0.5f,  0.0f,  0.5f,     1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
    0.0f,  0.8f,  0.0f,     0.0f, 0.5f, 0.5f,   0.5f, 1.0f,

};

std::vector<unsigned int> pyramidIndices = {
    0, 1, 2,
    0, 2, 3,
    0, 1, 4,
    1, 2, 4,
    2, 3, 4,
    3, 0, 4
};

std::vector<float> cubeVertices = {

    -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,
    
    -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,

};

std::vector<unsigned int> cubeIndices = {
    0, 1, 2,  2, 3, 0,
    4, 5, 6,  6, 7, 4,
    0, 3, 7,  7, 4, 0,
    1, 2, 6,  6, 5, 1,
    3, 2, 6,  6, 7, 3,
    0, 1, 5,  5, 4, 0 
};

// Dimensiones de la ventana
const unsigned int width = 800;
const unsigned int height = 600;

// Código de los shaders
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor; 
layout (location = 2) in vec2 aTex;

out vec3 ourColor;
out vec2 texCoord;

uniform mat4 CameraMatrix;
uniform mat4 ModelMatrix;

void main() {
    gl_Position = CameraMatrix * ModelMatrix * vec4(aPos, 1.0);
    ourColor = aColor;
    texCoord = aTex;
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec2 texCoord;
uniform sampler2D tex0;
void main() {
    FragColor = texture(tex0, texCoord);
}
)";

// Funciones auxiliares
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Error al inicializar GLAD" << std::endl;
        return -1;
    }

    // Crear y compilar shaders
    Shader vertexShader(vertexShaderSource, GL_VERTEX_SHADER);
    Shader fragmentShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

    ShaderProgram shaderProgram;
    shaderProgram.attachShader(vertexShader);
    shaderProgram.attachShader(fragmentShader);
    shaderProgram.linkProgram();

    // Crear malla para el cubo
    Mesh cubeMesh(cubeVertices, cubeIndices);
    Mesh pyramidMesh(pyramidVertices, pyramidIndices);

    // Cargar textura
    int widhtImg, heightImg, PixelNum;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load("chill.jpg", &widhtImg, &heightImg, &PixelNum, 0);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (bytes) {
        GLenum format = (PixelNum == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, widhtImg, heightImg, 0, format, GL_UNSIGNED_BYTE, bytes);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(bytes);

    // Configurar cámara
    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

    // Bucle de renderizado
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.use();
        camera.CameraMatrix(45.0f, 0.1f, 100.0f, shaderProgram.getID(), "CameraMatrix");
        camera.CameraInputs(window);

        // Dibujar el cubo
        cubeMesh.Draw(shaderProgram.getID(), glm::vec3(0.0f));
        pyramidMesh.Draw(shaderProgram.getID(), glm::vec3(2.0f, 1.0f, 0.0f));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
