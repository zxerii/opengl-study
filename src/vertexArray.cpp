#include "vertexArray.h"
#include "GLBasic.h"

VertexArray::VertexArray()
{
	GLCALL(glGenVertexArrays(1, &m_Renderer));
}

VertexArray::~VertexArray()
{
	GLCALL(glDeleteVertexArrays(1, &m_Renderer));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) 
	{
		const auto& element = elements[i];
		GLCALL(glEnableVertexAttribArray(i));
		GLCALL(glVertexAttribPointer(i, element.count, element.type, 
			element.normalized, layout.GetStride(), (const void *)offset));
		offset += element.count * VertexBufferElement::GetTypeSize(element.type);
	}
}

void VertexArray::Bind()
{
	GLCALL(glBindVertexArray(m_Renderer));
}

void VertexArray::UnBind()
{
	GLCALL(glBindVertexArray(0));
}
