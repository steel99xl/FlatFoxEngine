//
// Created by steel99xl on 6/14/21.
//
#pragma once
#include <GL/glew.h>
#include "FlatFoxRenderWarnings.hpp"
class IndexBuffer{
protected:
    unsigned int  m_RenderID;
    unsigned int m_Count;
    unsigned int m_Unquie;
public:
    IndexBuffer();
    ~IndexBuffer();

    void MakeDynamicBuffer(const unsigned int *data, unsigned int count);
    void MakeStaticBuffer(const unsigned int *data, unsigned int count);

    void Bind() const;
    void UnBind() const;

    inline unsigned int GetCount () const {return m_Count; }
    inline unsigned int Unquie() const {return m_Unquie; }
};




