#pragma once

#include <GLBasic.h>
#include <iostream>

//Shader Compiler
struct ProgramSource;

class Shader {
public:

	Shader(const std::string & filepath);
	~Shader();

	void Bind() const;
	void UnBind() const;

	template <typename T>
	void SetValue(const std::string& name, T value) const
	{
		//static_assert(false);
	}

	template <>
	void SetValue(const std::string& name, bool value) const
	{
		GLCALL(glUniform1i(glGetUniformLocation(m_ProgramID, name.c_str()), (int)value));
	}

	template <>
	void SetValue(const std::string& name, int value) const
	{
		GLCALL(glUniform1i(glGetUniformLocation(m_ProgramID, name.c_str()), value));
	}

	template <>
	void SetValue(const std::string& name, float value) const
	{
		GLCALL(glUniform1f(glGetUniformLocation(m_ProgramID, name.c_str()), value));
	}

private:
	int m_ProgramID;
};