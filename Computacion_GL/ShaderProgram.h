#pragma once

#include "Shader.h"
#include <glm/fwd.hpp>
class ShaderProgram {

public:
    unsigned int ID;

    ShaderProgram();

    void AttachShader(const Shader& shader);

    void Link();

    void Use() const;
    
    void SetVec3(const std::string& name, const glm::vec3& value) const;

    void SetMat4(const std::string& name, const glm::mat4& mat) const;
    
    void SetFloat(const std::string& name, float value) const;

    ~ShaderProgram();
};