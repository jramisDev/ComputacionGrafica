#pragma once

#include <glad/glad.h>
#include <string>
class ShaderProgram {
public:
    ShaderProgram();
    void attachShader(const Shader& shader);
    void linkProgram();
    void use() const;
    unsigned int getID() const;
    int getUniformLocation(const std::string& name);
    ~ShaderProgram();
private:
    unsigned int id;
};