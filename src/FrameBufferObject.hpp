#pragma once
#include "FlatFoxEngine.hpp"
#include "SimpleObject.hpp"

class FrameBufferObject{
    protected:
        unsigned int m_FBO;
        unsigned int m_FrameBufferTexture;
        unsigned int m_RBO;

        FlatFoxObject::SimpleObject m_Frame;

        int m_Width, m_Height;
        float m_TextureScale;



    public:

        FrameBufferObject();
        ~FrameBufferObject();

        void Setup(int width, int height, float scale, std::vector<std::string> ShaderVector);

        void Update(int width, int height, float scale);

        void Render();

        inline unsigned int GetFBO(){return m_FBO;}
        inline unsigned int GetTexture(){return m_FrameBufferTexture;}
        inline unsigned int GetRBO(){return m_RBO;}

};