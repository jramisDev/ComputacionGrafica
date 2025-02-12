#pragma once

class Mesh {
public:

    unsigned int VAO, VBO, EBO;

    size_t indexCount;

    Mesh(const float* vertices, size_t vertexSize, const unsigned int* indices, size_t indexSize);

    void Bind() const;

    ~Mesh();
};