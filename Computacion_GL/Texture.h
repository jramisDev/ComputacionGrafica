﻿#include <stb/stb_image.h>

class Texture {
public:
    unsigned int ID;

    Texture(const std::string& path, GLenum format = GL_RGB) {
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cerr << "Error: Failed to load texture at " << path << std::endl;
        }
        stbi_image_free(data);
    }

    void bind() const {
        glBindTexture(GL_TEXTURE_2D, ID);
    }

    ~Texture() {
        glDeleteTextures(1, &ID);
    }
};

