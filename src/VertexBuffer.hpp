//
// Created by steel99xl on 6/14/21.
//
#pragma once
#include "Imports/glew/include/GL/glew.h"
#include "FlatFoxRenderWarnings.hpp"
//#include "FlatFoxGL.hpp"

class VertexBuffer{
protected:
    unsigned int  m_RenderID;
public:
    VertexBuffer();
    ~VertexBuffer();

    void MakeDynamicBuffer(const void *data, unsigned int size);
    void MakeStaticBuffer(const void *data, unsigned int size);

    void Bind() const;
    void UnBind() const;

    inline unsigned int GetBufferID() { return m_RenderID; };
};





