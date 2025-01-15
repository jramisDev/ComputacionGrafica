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

// Clase Shader
//class Shader {
//public:
//    GLuint ID;
//    Shader(const std::string& source, GLenum shaderType) {
//        ID = glCreateShader(shaderType);
//        const char* src = source.c_str();
//        glShaderSource(ID, 1, &src, nullptr);
//        glCompileShader(ID);
//
//        GLint success;
//        glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
//        if (!success) {
//            char infoLog[512];
//            glGetShaderInfoLog(ID, 512, nullptr, infoLog);
//            std::cerr << "Error al compilar el shader: " << infoLog << std::endl;
//        }
//    }
//
//    ~Shader() {
//        glDeleteShader(ID);
//    }
//};
//
//// Clase ShaderProgram
//class ShaderProgram {
//public:
//    GLuint ID;
//
//    ShaderProgram() {
//        ID = glCreateProgram();
//    }
//
//    void attachShader(const Shader& shader) {
//        glAttachShader(ID, shader.ID);
//    }
//
//    void linkProgram() {
//        glLinkProgram(ID);
//
//        GLint success;
//        glGetProgramiv(ID, GL_LINK_STATUS, &success);
//        if (!success) {
//            char infoLog[512];
//            glGetProgramInfoLog(ID, 512, nullptr, infoLog);
//            std::cerr << "Error al vincular el programa: " << infoLog << std::endl;
//        }
//    }
//
//    void use() const {
//        glUseProgram(ID);
//    }
//
//    GLuint getUniformLocation(const std::string& name) const {
//        return glGetUniformLocation(ID, name.c_str());
//    }
//
//    void setInt(const std::string& name, int value) const {
//        glUniform1i(getUniformLocation(name), value);
//    }
//
//    void setMat4(const std::string& name, const glm::mat4& matrix) const {
//        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
//    }
//
//    ~ShaderProgram() {
//        glDeleteProgram(ID);
//    }
//};

// Datos para el modelo
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
void main() {
    gl_Position = CameraMatrix * vec4(aPos, 1.0);
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
        GLenum format = (PixelNum == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, widhtImg, heightImg, 0, format, GL_UNSIGNED_BYTE, bytes);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);

    shaderProgram.use();

    glEnable(GL_DEPTH_TEST);

    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.use();
        camera.CameraMatrix(45.0f, 0.1f, 100.0f, shaderProgram.getID(), "CameraMatrix");
        camera.CameraInputs(window);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
    glfwTerminate();
    return 0;
}
