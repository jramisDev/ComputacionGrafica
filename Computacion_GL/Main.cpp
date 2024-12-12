#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <STB/stb_image.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const unsigned int width = 800;
const unsigned int height = 600;

class Shader {
public:
    unsigned int ID;

    Shader(const char* source, GLenum shaderType) {
        ID = glCreateShader(shaderType);
        glShaderSource(ID, 1, &source, nullptr);
        glCompileShader(ID);

        int success;
        glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(ID, 512, nullptr, infoLog);
            std::cerr << "Error: Shader compilation failed\n" << infoLog << std::endl;
        }
    }

    ~Shader() {
        glDeleteShader(ID);
    }
};

class ShaderProgram {
public:
    unsigned int ID;

    ShaderProgram() {
        ID = glCreateProgram();
    }

    void attachShader(const Shader& shader) {
        glAttachShader(ID, shader.ID);
    }

    void link() {
        glLinkProgram(ID);

        int success;
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(ID, 512, nullptr, infoLog);
            std::cerr << "Error: Program linking failed\n" << infoLog << std::endl;
        }
    }

    void use() const {
        glUseProgram(ID);
    }

    ~ShaderProgram() {
        glDeleteProgram(ID);
    }
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

float vertices[] = {
    -0.5f, 0.0f,  0.5f,   0.0f, 0.0f,
     0.5f, 0.0f,  0.5f,   1.0f, 0.0f,
     0.5f, 0.0f, -0.5f,   1.0f, 1.0f,
    -0.5f, 0.0f, -0.5f,   0.0f, 1.0f,
     0.0f, 0.8f,  0.0f,   0.5f, 0.5f
};

unsigned int indices[] = {
    0, 1, 2,
    0, 2, 3,
    0, 1, 4,
    1, 2, 4,
    2, 3, 4,
    3, 0, 4
};

const char* vertexShaderSource = R"glsl(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aColor;
out vec2 TexCoord;
out vec3 ourColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ourColor = aColor;
    TexCoord = aTexCoord;
}
)glsl";

const char* fragmentShaderSource = R"glsl(
#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
in vec3 ourColor;
uniform sampler2D tex0;

void main() {
    FragColor = texture(tex0, TexCoord);
}
)glsl";

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Computacion Grafica", nullptr, nullptr);
    if (!window) {
        std::cerr << "Error al crear la ventana" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwMakeContextCurrent(window);

    //Inicializar GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Error al inicializar GLAD" << std::endl;
        return -1;
    }
    
    //Crear shaders y programa
    Shader vertexShader(vertexShaderSource, GL_VERTEX_SHADER);
    Shader fragmentShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    ShaderProgram shaderProgram;
    shaderProgram.attachShader(vertexShader);
    shaderProgram.attachShader(fragmentShader);
    shaderProgram.link();

    //Configuración de buffers
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //Solo para el triangulo
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Cargar textura
    int widthImg, heightImg, PixelNum;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load("chill.jpg", &widthImg, &heightImg, &PixelNum, 0);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(bytes);

    glEnable(GL_DEPTH_TEST);

    // Bucle principal
    while (!glfwWindowShouldClose(window)) {

        glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //shaderProgram.use();

        glm::mat4 model = glm::mat4(1.f);
        glm::mat4 view = glm::mat4(1.f);
        glm::mat4 projection = glm::mat4(1.f);

        //CAMBIAR ROTACION A DELTA TIME !!
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, -0.1f, -2.0f));

        //CONFIGURACION DE LA CAMARA
        projection = glm::perspective(glm::radians(45.0f), (float)(width/height), 0.1f, 100.f);

        GLuint modelUniform = glGetUniformLocation(shaderProgram.ID, "model");
        glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));

        GLuint viewUniform = glGetUniformLocation(shaderProgram.ID, "view");
        glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(view));

        GLuint projectionUniform = glGetUniformLocation(shaderProgram.ID, "projection");
        glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(projection));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Limpieza
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    shaderProgram.~ShaderProgram();
    glfwTerminate();
    return 0;
}