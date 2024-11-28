#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shaders/Shader.h"
#include "ShadersProgram/ShaderProgram.h"

int alternativeColorLocation;

void InstallShaders(const std::string& vertexCode, const std::string& FragmentCode);

std::string ReadTextFile(const std::string& FileName);

// Código fuente de los shaders
const char* vertexShaderSource;

const char* fragmentShaderSource;

void frameBuffer_size_callback(GLFWwindow* Window, int width, int height) {
    glViewport(0, 0, width, height);
}

void InstallShaders(const std::string& vertexCode, const std::string& FragmentCode)
{
    vertexShaderSource = vertexCode.c_str();
    fragmentShaderSource = FragmentCode.c_str();
}

float vertices[] = {
    // VERTICE                  COLOR
     0.000f,  0.000f, 0.000f,   1.f, 0.f, 0.f, //0 
     0.150f,  0.400f, 0.000f,   1.f, 0.f, 0.f, //1
    -0.150f,  0.400f, 0.000f,   1.f, 0.f, 0.f, //2
    -0.500f,  0.280f, 0.000f,   1.f, 0.f, 0.f, //3
    -0.120f, -0.010f, 0.000f,   1.f, 0.f, 0.f, //4
    -0.600f,  0.700f, 0.000f,   1.f, 0.f, 0.f, //5
    -0.600f, -0.060f, 0.000f,   1.f, 0.f, 0.f, //6
    -0.240f, -0.310f, 0.000f,   1.f, 0.f, 0.f, //7
    -0.250f, -0.022f, 0.000f,   1.f, 0.f, 0.f, //8
    -0.090f, -0.150f, 0.000f,   1.f, 0.f, 0.f, //9
    -0.040f, -0.450f, 0.000f,   1.f, 0.f, 0.f, //10
    -0.000f, -0.380f, 0.000f,   1.f, 0.f, 0.f, //11
    -0.000f, -0.470f, 0.000f,   1.f, 0.f, 0.f, //12
     0.040f, -0.450f, 0.000f,   1.f, 0.f, 0.f, //13
     0.120f, -0.010f, 0.000f,   1.f, 0.f, 0.f, //14
     0.240f, -0.310f, 0.000f,   1.f, 0.f, 0.f, //15
     0.090f, -0.150f, 0.000f,   1.f, 0.f, 0.f, //16
     0.250f, -0.022f, 0.000f,   1.f, 0.f, 0.f, //17
     0.600f, -0.060f, 0.000f,   1.f, 0.f, 0.f, //18
     0.500f,  0.280f, 0.000f,   1.f, 0.f, 0.f, //19
     0.600f,  0.700f, 0.000f,   1.f, 0.f, 0.f, //20
};

unsigned int indices[] = {
    0,1,2,          //1
    0,2,3,          //2
    0,3,4,          //3
    2,5,3,          //4
    4,3,6,          //5
    6,7,8,          //6
    8,9,4,          //7
    7,9,8,          //8
    7,10,9,         //9
    10,11,4,        //10
    11,0,4,         //11
    10,12,11,       //12
    12,13,11,       //13
    11,14,0,        //14
    11,13,14,       //15
    13,15,16,       //16
    15,17,16,       //17
    16,17,14,       //18
    15,18,17,       //19
    18,19,14,       //20
    14,19,0,        //21
    0,19,1,         //22
    19,20,1,        //23
};

int main(void)
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* Window = glfwCreateWindow(800, 800, "Computacion Grafica", nullptr, nullptr);

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

    InstallShaders(ReadTextFile("vertex.glsl"), ReadTextFile("fragment.glsl"));

    Shader vertexShader(vertexShaderSource, GL_VERTEX_SHADER);
    Shader fragmentShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

    ShaderProgram shaderProgram;
    shaderProgram.attachShader(vertexShader);
    shaderProgram.attachShader(fragmentShader);
    shaderProgram.link();

    alternativeColorLocation = shaderProgram.getUniformLocation("alternativeColor");

    unsigned int VBO, VAO, IBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    GLsizei stride = 0;
    void* offset = nullptr;
    //int* offsetDraw = 0;

    stride = 6 * sizeof(float);
    offset = (void*)0;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(0);

    stride = 6 * sizeof(float);
    offset = (void*)(3 * sizeof(float));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(1);

    //Wireframe activado
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    int divIndex = 23;

    float pielR = 1.000f;
    float pielG = 0.600f;
    float pielB = 0.000f;

    float orejasR = 0.969f;
    float orejasG = 0.686f;
    float orejasB = 0.259f;

    float narizOjosR = 0.0f;
    float narizOjosG = 0.0f;
    float narizOjosB = 0.0f;

    float morroR = 0.839f;
    float morroG = 0.506f;
    float morroB = 0.000;

    while (!glfwWindowShouldClose(Window)) {

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        shaderProgram.use();
        glBindVertexArray(VAO);

        //1
        glUniform4f(alternativeColorLocation, pielR, pielG, pielB, 1.0f);
        glDrawElements(GL_TRIANGLES, std::size(indices) / divIndex, GL_UNSIGNED_INT, 0);
        //2
        glUniform4f(alternativeColorLocation, pielR, pielG, pielB, 1.0f);
        glDrawElements(GL_TRIANGLES, std::size(indices) / divIndex, GL_UNSIGNED_INT, (int*)NULL + 3);
        //3
        glUniform4f(alternativeColorLocation, pielR, pielG, pielB, 1.0f);
        glDrawElements(GL_TRIANGLES, std::size(indices) / divIndex, GL_UNSIGNED_INT, (int*)NULL + 6);
        //4
        glUniform4f(alternativeColorLocation, morroR, morroG, morroB, 1.0f);
        glDrawElements(GL_TRIANGLES, std::size(indices) / divIndex, GL_UNSIGNED_INT, (int*)NULL + 9);
        //5
        glUniform4f(alternativeColorLocation, pielR, pielG, pielB, 1.0f);
        glDrawElements(GL_TRIANGLES, std::size(indices) / divIndex, GL_UNSIGNED_INT, (int*)NULL + 12);
        //6
        glUniform4f(alternativeColorLocation, pielR, pielG, pielB, 1.0f);
        glDrawElements(GL_TRIANGLES, std::size(indices) / divIndex, GL_UNSIGNED_INT, (int*)NULL + 15);
        //7
        glUniform4f(alternativeColorLocation, narizOjosR, narizOjosG, narizOjosB, 1.0f);
        glDrawElements(GL_TRIANGLES, std::size(indices) / divIndex, GL_UNSIGNED_INT, (int*)NULL + 18);
        //8
        glUniform4f(alternativeColorLocation, pielR, pielG, pielB, 1.0f);
        glDrawElements(GL_TRIANGLES, std::size(indices) / divIndex, GL_UNSIGNED_INT, (int*)NULL + 21);
        //9
        glUniform4f(alternativeColorLocation, pielR, pielG, pielB, 1.0f);
        glDrawElements(GL_TRIANGLES, std::size(indices) / divIndex, GL_UNSIGNED_INT, (int*)NULL + 24);
        //10
        glUniform4f(alternativeColorLocation, morroR, morroG, morroB, 1.0f);
        glDrawElements(GL_TRIANGLES, std::size(indices) / divIndex, GL_UNSIGNED_INT, (int*)NULL + 27);
        //11
        glUniform4f(alternativeColorLocation, morroR, morroG, morroB, 1.0f);
        glDrawElements(GL_TRIANGLES, std::size(indices) / divIndex, GL_UNSIGNED_INT, (int*)NULL + 30);
        //12
        glUniform4f(alternativeColorLocation, narizOjosR, narizOjosG, narizOjosB, 1.0f);
        glDrawElements(GL_TRIANGLES, std::size(indices) / divIndex, GL_UNSIGNED_INT, (int*)NULL + 33);
        //13
        glUniform4f(alternativeColorLocation, narizOjosR, narizOjosG, narizOjosB, 1.0f);
        glDrawElements(GL_TRIANGLES, std::size(indices) / divIndex, GL_UNSIGNED_INT, (int*)NULL + 36);
        //14
        glUniform4f(alternativeColorLocation, morroR, morroG, morroB, 1.0f);
        glDrawElements(GL_TRIANGLES, std::size(indices) / divIndex, GL_UNSIGNED_INT, (int*)NULL + 39);
        //15
        glUniform4f(alternativeColorLocation, morroR, morroG, morroB, 1.0f);
        glDrawElements(GL_TRIANGLES, std::size(indices) / divIndex, GL_UNSIGNED_INT, (int*)NULL + 42);
        //16
        glUniform4f(alternativeColorLocation, pielR, pielG, pielB, 1.0f);
        glDrawElements(GL_TRIANGLES, std::size(indices) / divIndex, GL_UNSIGNED_INT, (int*)NULL + 45);
        //17
        glUniform4f(alternativeColorLocation, pielR, pielG, pielB, 1.0f);
        glDrawElements(GL_TRIANGLES, std::size(indices) / divIndex, GL_UNSIGNED_INT, (int*)NULL + 48);
        //18
        glUniform4f(alternativeColorLocation, narizOjosR, narizOjosG, narizOjosB, 1.0f);
        glDrawElements(GL_TRIANGLES, std::size(indices) / divIndex, GL_UNSIGNED_INT, (int*)NULL + 51);
        //19
        glUniform4f(alternativeColorLocation, pielR, pielG, pielB, 1.0f);
        glDrawElements(GL_TRIANGLES, std::size(indices) / divIndex, GL_UNSIGNED_INT, (int*)NULL + 54);
        //20
        glUniform4f(alternativeColorLocation, pielR, pielG, pielB, 1.0f);
        glDrawElements(GL_TRIANGLES, std::size(indices) / divIndex, GL_UNSIGNED_INT, (int*)NULL + 57);
        //21
        glUniform4f(alternativeColorLocation, pielR, pielG, pielB, 1.0f);
        glDrawElements(GL_TRIANGLES, std::size(indices) / divIndex, GL_UNSIGNED_INT, (int*)NULL + 60);
        //22
        glUniform4f(alternativeColorLocation, pielR, pielG, pielB, 1.0f);
        glDrawElements(GL_TRIANGLES, std::size(indices) / divIndex, GL_UNSIGNED_INT, (int*)NULL + 63);
        //23
        glUniform4f(alternativeColorLocation, morroR, morroG, morroB, 1.0f);
        glDrawElements(GL_TRIANGLES, std::size(indices) / divIndex, GL_UNSIGNED_INT, (int*)NULL + 66);

        glfwSwapBuffers(Window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);

    glfwTerminate();
    return 0;
}

std::string ReadTextFile(const std::string& FileName) {

    std::ifstream File(FileName);
    if (!File.is_open())
        return "";

    std::stringstream stringbuffer{};
    stringbuffer << File.rdbuf();
    File.close();

    return stringbuffer.str();
}