#include "IndexBuffer.hpp"


IndexBuffer::IndexBuffer() {

}

IndexBuffer::~IndexBuffer(){
	GLCall(glDeleteBuffers(1, &m_RenderID));
}


void IndexBuffer::MakeDynamicBuffer(const unsigned int *data, unsigned int count){
		m_Count = count;
		m_Unquie = sizeof(data);

		GLCall(glGenBuffers(1, &m_RenderID));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count , data, GL_DYNAMIC_DRAW));

}


void IndexBuffer::MakeStaticBuffer(const unsigned int *data, unsigned int count){
		m_Count = count;
		m_Unquie = sizeof(data);

		GLCall(glGenBuffers(1, &m_RenderID));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count , data, GL_STATIC_DRAW));

}

void IndexBuffer::Bind() const {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID));
}

void IndexBuffer::UnBind() const {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
