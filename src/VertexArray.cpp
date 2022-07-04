#include "VertexArray.hpp"

VertexArray::VertexArray(){
    GLCall(glGenVertexArrays(1,&m_RenderID));
    
}

VertexArray::~VertexArray(){

    GLCall(glDeleteVertexArrays(1, &m_RenderID));

}

void VertexArray::Bind() const{
    GLCall(glBindVertexArray(m_RenderID));
}

void VertexArray::UnBind() const{
    GLCall(glBindVertexArray(0));
}

void VertexArray::AddBuffer(const class VertexBuffer &vb, const VertexBufferLayout &layout){

    Bind();

    vb.Bind();

    const auto &elements = layout.GetElements();

    unsigned int offset = 0;

    for(unsigned int i = 0; i < elements.size(); i++ ){
        const auto &element = elements[i]
        GLCall(glEnableVertexAttribArray(i));
        GLCall(glVertexAttribPointer(i,element.count, element.type, element.normalized, layout.GetStride() , (const void*)(size_t)offset ));
        offset += element.count *  VertexBufferElements::GetSizeOfType(element.type);
    }

}