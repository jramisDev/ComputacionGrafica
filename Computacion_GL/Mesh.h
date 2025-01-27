#pragma once

#include <glad/glad.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "Texture.h"

class Mesh {
public:
    Mesh(std::vector<float> vertices, std::vector<unsigned int> indices, const std::string& texturePath);
    ~Mesh();

    void Draw(unsigned int InShaderID, const glm::vec3 Offset) const;

private:
    unsigned int VAO, VBO, IBO;
    unsigned long long sizeIndices;
    Texture texture;  // Añadimos la textura a la malla

    void SetupMesh(std::vector<float> vertices, std::vector<unsigned int> indices);
};
