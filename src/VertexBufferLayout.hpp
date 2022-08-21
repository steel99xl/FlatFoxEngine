#pragma once

#include <vector>
#include <cstring>

#include <GL/glew.h>
#include "FlatFoxRenderWarnings.hpp"

#include "VertexBuffer.hpp"
//#include "FlatFoxGL.hpp"

struct VertexBufferElements{
    unsigned int type;
    unsigned int count;
    bool normalized;
    static unsigned int GetSizeOfType(unsigned int type){

        switch(type){
            case GL_FLOAT: return 4;
            case GL_UNSIGNED_INT: return 4;
            case GL_UNSIGNED_BYTE: return 1;
        }
        return 0;

    }
};


class VertexBufferLayout{
protected:
    std::vector<VertexBufferElements> m_Elements;
    unsigned int m_Stride;

public:

    VertexBufferLayout()
            : m_Stride(0) { }

    //template<typename T>
    void Push(unsigned int count, std::string type){
        if(std::strcmp(type.c_str(),"float") == 0){
            m_Elements.push_back({GL_FLOAT, count, false});
            m_Stride += count * VertexBufferElements::GetSizeOfType(GL_FLOAT);
        }
        if(std::strcmp(type.c_str(),"unsigned int") == 0){
            m_Elements.push_back({GL_UNSIGNED_INT, count, false});
            m_Stride += count * VertexBufferElements::GetSizeOfType(GL_UNSIGNED_INT);
        }
        if(std::strcmp(type.c_str(),"unsigned char") == 0){
            m_Elements.push_back({GL_UNSIGNED_BYTE, count, true});
            m_Stride += count * VertexBufferElements::GetSizeOfType(GL_UNSIGNED_BYTE);
        }
        //static_assert(false);
    }

    inline const std::vector<VertexBufferElements> GetElements() const& {return m_Elements;};

    inline unsigned int GetStride() const& {return m_Stride;}
};




        
