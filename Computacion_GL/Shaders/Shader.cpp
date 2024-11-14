#include "Shader.h"
#include <iostream>

Shader::Shader(const std::string& source, GLenum type) {
    id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
}

unsigned int Shader::getID() const {
    return id;
}

Shader::~Shader() {
    glDeleteShader(id);
}