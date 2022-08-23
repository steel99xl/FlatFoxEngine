#pragma once
#include "Renderer.hpp"
#include "stb_image.h"
#include "FlatFoxRenderWarnings.hpp"
//#include "FlatFoxGL.hpp"

class Texture{
    protected:
        unsigned int m_RendererID;
        GLuint m_CurrentTexture[32];
        std::string m_FilePath;
        unsigned char *m_LocalBuffer;
        int m_Width, m_Height, m_BPP;

    public:
        Texture();
        ~Texture();

        void Bind(unsigned int slot = 0) const;
        void UnBind() const;

        void LoadTexture(const std::string &filePath, unsigned int slot = 0);

        unsigned int MakeTexture(const std::string &filePath, int width = 0, int hegith = 0);


        inline int GetWidth() const {return m_Width;}

        inline int GetHeight() const {return m_Height;}

        inline GLuint GetTexture() const {return m_RendererID;}


};