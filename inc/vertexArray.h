#pragma once

#include "vertexBuffer.h"
#include "vertexBufferLayout.h"

class VertexArray
{
private:
	unsigned int m_Renderer;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void Bind();
	void UnBind();
};