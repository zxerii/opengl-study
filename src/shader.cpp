#include "shader.h"
#include <fstream>
#include <sstream>
#include <string>

//Shader Compiler
struct ProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

static ProgramSource ParseShader(const std::string& filepath);
static unsigned int createShader(std::string vertex, std::string fragment);

Shader::Shader(const std::string& filepath)
{
    int vertex, fragment;
    ProgramSource src = ParseShader(filepath);
    std::cout << "VERTEX : " << std::endl;
    std::cout << src.VertexSource << std::endl;
    std::cout << "FRAGMENT : " << std::endl;
    std::cout << src.FragmentSource << std::endl;
    m_ProgramID = createShader(src.VertexSource, src.FragmentSource);    
}

Shader::~Shader()
{
    GLCALL(glDeleteProgram(m_ProgramID));
}

void Shader::Bind() const
{
	GLCALL(glUseProgram(m_ProgramID));
}

void Shader::UnBind() const
{
	GLCALL(glUseProgram(0));
}


static ProgramSource ParseShader(const std::string& filepath)
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
unsigned int compileShader(unsigned int type, std::string sourceCode)
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
    int ret;
    glGetProgramiv(program, GL_LINK_STATUS, &ret);
    if (ret == GL_FALSE) {
        int length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetProgramInfoLog(program, length, &length, message);
        std::cout << "Link Program Error\n";
        std::cout << message << std::endl;
        return 0;
    }
    glValidateProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}