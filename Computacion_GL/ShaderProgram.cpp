#include "ShaderProgram.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

ShaderProgram::ShaderProgram()
{
	ID = glCreateProgram();
}

void ShaderProgram::AttachShader(const Shader& shader)
{
	glAttachShader(ID, shader.ID);
}

void ShaderProgram::Link()
{
    glLinkProgram(ID);

    int success;
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        std::cerr << "Error: Program linking failed\n" << infoLog << std::endl;
    }
}

void ShaderProgram::Use() const
{
    glUseProgram(ID);
}

void ShaderProgram::SetVec3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void ShaderProgram::SetMat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void ShaderProgram::SetFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(ID);
}
