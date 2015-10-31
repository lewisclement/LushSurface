#include "shader.hpp"

Shader::Shader(std::string vertexShaderPath, std::string fragmentShaderPath) {
    GLuint vertexShader = loadShader(vertexShaderPath, GL_VERTEX_SHADER);
    GLuint fragmentShader = loadShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(programID);
}

void Shader::activate() {
    glUseProgram(programID);
}

GLuint loadShader(std::string filepath, GLuint in_type) {
    std::ifstream file(filepath.c_str(), std::ifstream::in);
    if (!file.is_open()) {
        std::cout << "Couldn't open shader file " << filepath << std::endl;
        return 0;
    }

    std::string lines, line;

    while (file.good()) {
        std::getline(file, line);
        lines.append(line);
        lines.append("\n");
    }
    file.close();
    const char* shaderContent = lines.c_str();

    GLuint shader = glCreateShader(in_type);
    glShaderSource(shader, 1, &shaderContent, NULL);
    glCompileShader(shader);

    //Error checks
    GLint success = GL_FALSE;
    GLint infoLogLength = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
    if(!success || infoLogLength > 1) {
        std::vector<GLchar> ShaderErrorMessage((unsigned long) infoLogLength);
        glGetShaderInfoLog(shader, infoLogLength, NULL, &ShaderErrorMessage[0]);

        std::cout << "Messages while compiling " << filepath << ": \n";
        fprintf (stdout, "%s\n", &ShaderErrorMessage[0]);

        if(!success) {
            glDeleteShader(shader);
            return 0;
        }
    }

    return shader;
}
