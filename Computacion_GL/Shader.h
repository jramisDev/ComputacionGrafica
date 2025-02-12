#pragma once

#include <iostream>
#include "Dependencies/include/glad/glad.h"

class Shader {
public:
    unsigned int ID;

    Shader(const char* source, GLenum shaderType);

    ~Shader();
};
