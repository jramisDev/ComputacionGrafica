#include "Mesh.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Mesh::Mesh(std::vector<float> vertices, std::vector<unsigned int> indices) {
    SetupMesh(vertices, indices);

    sizeIndices = sizeof(indices);
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
}

void Mesh::SetupMesh(std::vector<float> vertices, std::vector<unsigned int> indices) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);

    glBindVertexArray(VAO);

    // VBO: Ver texto y coordenadas de los vértices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // IBO: Índices de los vértices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Configurar los atributos de los vértices
    unsigned int stride = 8 * sizeof(float);  // 3 (posición) + 3 (color) + 2 (textura)
    
    // Posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Textura
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::Draw(unsigned int InShaderID, const glm::vec3 Offset) const {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, sizeIndices, GL_UNSIGNED_INT, 0);  // 36 porque un cubo tiene 12 triángulos
    glBindVertexArray(0);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), Offset);  // Desplazado a la derecha
    GLuint modelLoc = glGetUniformLocation(InShaderID, "ModelMatrix");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}
