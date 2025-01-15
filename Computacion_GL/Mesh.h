#pragma once

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>

class Mesh {
public:
    Mesh(std::vector<float> vertices, std::vector<unsigned int> indices);
    ~Mesh();

    void Draw(unsigned int InShaderID, const glm::vec3 Offset) const;

private:
    unsigned int VAO, VBO, IBO;
    unsigned long long sizeIndices;
    void SetupMesh(std::vector<float> vertices, std::vector<unsigned int> indices);
};