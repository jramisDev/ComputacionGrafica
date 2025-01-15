#include "Shader.h"
#include "ShaderProgram.h"
#include <iostream>

ShaderProgram::ShaderProgram() {
    id = glCreateProgram();
}
void ShaderProgram::attachShader(const Shader& shader) {
    glAttachShader(id, shader.getID());
}
void ShaderProgram::linkProgram() {
    glLinkProgram(id);
}
void ShaderProgram::use() const {
    glUseProgram(id);
}
unsigned int ShaderProgram::getID() const {
    return id;
}
int ShaderProgram::getUniformLocation(const std::string& name) {
    return glGetUniformLocation(id, name.c_str());
}
ShaderProgram::~ShaderProgram() {
    glDeleteProgram(id);
}