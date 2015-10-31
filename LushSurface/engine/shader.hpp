#ifndef SHADER_H
#define SHADER_H

#include "../pch.hpp"

class Shader {
public:
    Shader(std::string vertexShaderPath, std::string fragmentShaderPath);
    ~Shader();

    void activate();

    GLuint programID;

private:

};

GLuint loadShader(std::string filepath, GLuint in_type);

#endif // SHADER_H
