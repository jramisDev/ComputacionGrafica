
#pragma once

#include <glad/glad.h>
#include <string>
#include <stb/stb_image.h>
#include <iostream>

class Texture {
public:
    Texture(const std::string& path);
    void Bind() const;
    ~Texture();

private:
    unsigned int id;
};
