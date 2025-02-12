#pragma once

#include <stb/stb_image.h>
#include <glad/glad.h>

#include <string>

class Texture {
public:
    unsigned int ID;

    Texture(const std::string& path, GLenum format = GL_RGB);

    void Bind() const;

    ~Texture();
};

