#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.h"
#include "Texture.h"
#include "ShaderProgram.h"

class Mesh {
public:

    unsigned int VAO, VBO, EBO;

    size_t indexCount;

    Texture texture;
    glm::mat4 modelMatrix;

    Mesh(const float* vertices, size_t vertexSize, const unsigned int* indices, size_t indexSize, const std::string& texturePath) :
        texture(texturePath) {
        modelMatrix = glm::mat4(1.0f);

        indexCount = indexSize / sizeof(unsigned int);

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }


    void Bind() const;

    void setModelMatrix(const glm::mat4& matrix) {
        modelMatrix = matrix;
    }

    glm::vec3 getPosition()
    {
        return glm::vec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]);
    }

    glm::mat4 getModelMatrix()
    {
        return modelMatrix;
    }

    void draw(ShaderProgram& shaderProgram) {
        int modelLoc = glGetUniformLocation(shaderProgram.ID, "ModelMatrix");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        texture.Bind();
        Bind();
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    }

    ~Mesh();
};