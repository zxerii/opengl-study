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
