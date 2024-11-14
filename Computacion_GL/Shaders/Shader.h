#include <glad/glad.h>
#include <string>

class Shader {
public:
    Shader(const std::string& source, GLenum type);
    unsigned int getID() const;
    ~Shader();

private:
    unsigned int id;
};