#include "Texture.hpp"
#define STB_IMAGE_IMPLEMENTATIO


Texture::Texture() : m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0){
    

}

Texture::~Texture(){

    GLCall(glDeleteTextures(1, &m_RendererID));

}

void Texture::Bind(unsigned int slot) const{

    GLCall(glActiveTexture(GL_TEXTURE0 + slot));

    GLCall(glBindTexture(GL_TEXTURE_2D, m_CurrentTexture[slot]));

}

void Texture::UnBind() const{

    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

}

void Texture::LoadTexture(const std::string &filePath, unsigned int slot){
    stbi_set_flip_vertically_on_load(1);
    int width, height, bpp;
    GLuint TempTexture;
    m_LocalBuffer = stbi_load(filePath.c_str(), &width, &height, &bpp, 4);

    //std::cout << m_Width << std::endl;
    //std::cout << m_Height << std::endl;

    GLCall(glGenTextures(1, &TempTexture));
    GLCall(glBindTexture(GL_TEXTURE_2D, TempTexture)); 

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));


    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer ));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    if(m_LocalBuffer){
        stbi_image_free(m_LocalBuffer);
    }

    //std::cout << TempTexture << std::endl;
    m_CurrentTexture[slot] = TempTexture;


    GLCall(glActiveTexture(GL_TEXTURE0 + slot));

    GLCall(glBindTexture(GL_TEXTURE_2D, m_CurrentTexture[slot]));



}


unsigned int Texture::MakeTexture(const std::string &filePath, int width, int height){
    
    unsigned int TempTexture;

    stbi_set_flip_vertically_on_load(1);
    int bpp;
    GLCall(glGenTextures(1, &TempTexture));
    GLCall(glBindTexture(GL_TEXTURE_2D, TempTexture)); 

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));


    if(filePath == "0" || filePath == "NULL"){

        std::cout << width << " | " << height << std::endl;
        
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL ));       

    } else{

        m_LocalBuffer = stbi_load(filePath.c_str(), &width, &height, &bpp, 4);
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer )); 

    }

        return TempTexture;
    //std::cout << m_Width << std::endl;
    //std::cout << m_Height << std::endl;
}