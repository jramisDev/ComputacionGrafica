#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Object.h"

#include "Objects/Cube.h"
#include "Objects/Plane.h"

const char* vertexShaderSource = R"glsl(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 CameraMatrix;
uniform mat4 ModelMatrix;

void main() {
    gl_Position = CameraMatrix * ModelMatrix * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
    FragPos = vec3(ModelMatrix * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(ModelMatrix))) * aNormal;
}
)glsl";

const char* fragmentShaderSource = R"glsl(
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D tex0;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 lightPos2;  // Segunda luz
uniform vec3 lightColor2;  // Color de la segunda luz
uniform vec3 cameraPos;

// Atenuación de la luz (constant, linear, quadratic)
uniform float constant;
uniform float linear;
uniform float quadratic;

void main() {
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // Luz 2 - Point light
    vec3 lightDir2 = normalize(lightPos2 - FragPos);
    float diff2 = max(dot(norm, lightDir2), 0.0);
    vec3 diffuse2 = diff2 * lightColor2;
    vec3 reflectDir2 = reflect(-lightDir2, norm);
    float spec2 = pow(max(dot(viewDir, reflectDir2), 0.0), 32);
    vec3 specular2 = specularStrength * spec2 * lightColor2;

    // Calcular la atenuación de la luz puntual
    float distance = length(lightPos2 - FragPos);  // Distancia entre el fragmento y la luz
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance)); // Atenuación

    // Aplicar la atenuación a la luz 2
    diffuse2 *= attenuation;
    specular2 *= attenuation;

    // Sumar la contribución de ambas luces
    vec3 result = (ambient + diffuse + specular + diffuse2 + specular2) * vec3(texture(tex0, TexCoord));
    FragColor = vec4(result, 1.0);
}
)glsl";

const unsigned int width = 800, height = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(width, height, "Computacion Grafica", nullptr, nullptr);
    if (!window) {
        std::cerr << "Error al crear la ventana GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Error al inicializar GLAD" << std::endl;
        return -1;
    }

    Shader vertexShader(vertexShaderSource, GL_VERTEX_SHADER);
    Shader fragmentShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    ShaderProgram shaderProgram;
    shaderProgram.attachShader(vertexShader);
    shaderProgram.attachShader(fragmentShader);
    shaderProgram.link();
    glEnable(GL_DEPTH_TEST);

    // Crear objetos
    Object object1(verticesSuelo, sizeof(verticesSuelo), indicesSuelo, sizeof(indicesSuelo), "chill.jpg");
    Object object2(verticescubo, sizeof(verticescubo), indicescubo, sizeof(indicescubo), "chill.jpg");
    Object object3(verticescubo, sizeof(verticescubo), indicescubo, sizeof(indicescubo), "chill.jpg");
    Object object4(verticescubo, sizeof(verticescubo), indicescubo, sizeof(indicescubo), "chill.jpg");

    // Ajustar posición de los objetos
    object1.setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))); //Suelo
    object2.setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f))); //Fuente de iluminación
    object3.setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f))); //Cubo purpura
    object4.setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 2.0f, -2.0f))); //Fuente de iluminación

    // Crear cámara
    Camera camera(width, height, glm::vec3(0.0f, 1.0f, 4.0f));

    // Parámetros de atenuación de la luz puntual
    float constant = 1.0f;
    float linear = 0.05f;   // Reduce la atenuación
    float quadratic = 0.02f;

    // Bucle principal
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        camera.CameraInputs(window);

        shaderProgram.setVec3("viewPos", camera.Position);
        shaderProgram.setVec3("lightPos", object2.getPosition());  // Primera luz
        shaderProgram.setVec3("lightPos2", object4.getPosition());  // Segunda luz
        shaderProgram.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));  // Color de la primera luz
        shaderProgram.setVec3("lightColor2", glm::vec3(1.0f, 0.0f, 0.0f));  // Color de la segunda luz

        // Pasar los valores de atenuación al shader
        shaderProgram.setFloat("constant", constant);
        shaderProgram.setFloat("linear", linear);
        shaderProgram.setFloat("quadratic", quadratic);

        shaderProgram.setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
        shaderProgram.use();

        camera.CameraMatrix(45.0f, 0.1f, 100.f, shaderProgram.ID, "CameraMatrix");

        // Dibujar los objetos
        object1.draw(shaderProgram);
        object2.draw(shaderProgram);
        object3.draw(shaderProgram);
        object4.draw(shaderProgram);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
