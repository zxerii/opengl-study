#include "GLBasic.h"

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLGetError(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError()) {
		std::cout << "[ OpenGL Error ] : " << "(" << error << ")\nFunction : " << function \
			<< "\nLine : " << line << ", File : " << file << std::endl;
		return false;
	}
	return true;
}

ProgramSource ParseShader(const std::string& filepath)
{
    std::ifstream text(filepath);
    std::string line;
    std::stringstream ss[2];

    enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };
    ShaderType type = ShaderType::NONE;
    while (getline(text, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            ss[int(type)] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str() };
}

//在GPU上编译着色器
static unsigned int compileShader(unsigned int type, std::string sourceCode)
{
    unsigned int id = glCreateShader(type);
    const char* src = sourceCode.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int ret;
    glGetShaderiv(id, GL_COMPILE_STATUS, &ret);
    if (ret == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "shader compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "error : ";
        std::cout << message << std::endl;
        return 0;
    }
    return id;
}
//编译链接生成程序
unsigned int createShader(std::string vertex, std::string fragment)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertex);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragment);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    return program;
}