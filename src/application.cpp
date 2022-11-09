#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

//parse shader
#include <fstream>
#include <sstream>
#include <string>

struct ProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

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
static unsigned int createShader(std::string vertex, std::string fragment)
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

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(600, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //glew
    if (glewInit() != GLEW_OK)
        std::cout << "Error" << std::endl;
    std::cout << glGetString(GL_VERSION) << std::endl;

    //create and bind buffer
    float tre_vertex[] = {
        -0.5f, -0.5f,
        -0.5f,  0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f
    };

    unsigned int indices[] = {
        0, 1, 2,
        1, 2, 3
    };

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), tre_vertex, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint ind;
    glGenBuffers(1, &ind);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ind);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
    
    ProgramSource programsrc = ParseShader("shaders/Basic.shader");
    std::cout << "VERTEX : " << std::endl;
    std::cout << programsrc.VertexSource << std::endl;
    std::cout << "FRAGMENT : " << std::endl;
    std::cout << programsrc.FragmentSource << std::endl;
    unsigned int shader = createShader(programsrc.VertexSource, programsrc.FragmentSource);
    glUseProgram(shader);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        //glBegin(GL_TRIANGLES);

        //glVertex2f(-0.5, -0.5);
        //glVertex2f(0, 0.5);
        //glVertex2f(0.5, -0.5);

        //glEnd();
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}