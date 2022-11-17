#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <iostream>

#define ASSERT(x)		if(!(x)) __debugbreak();
#define GLCALL(x)		GLClearError();\
						x;\
						ASSERT(GLGetError(#x, __FILE__, __LINE__));

//GL ERROR LOG
void GLClearError();
bool GLGetError(const char* function, const char* file, int line);

//Shader Compiler
#include <fstream>
#include <sstream>
#include <string>

struct ProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

static unsigned int compileShader(unsigned int type, std::string sourceCode);
ProgramSource ParseShader(const std::string& filepath);
unsigned int  createShader(std::string vertex, std::string fragment);