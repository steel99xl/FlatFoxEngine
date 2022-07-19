#include "FrameBufferObject.hpp"

FrameBufferObject::FrameBufferObject(){}

FrameBufferObject::~FrameBufferObject(){}

void FrameBufferObject::Setup(int width, int height, float scale, std::vector<std::string> ShaderVector){
    glGenFramebuffers(1, &m_FBO);
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER ,m_FBO));


    glGenTextures(1, &m_FrameBufferTexture);
    glBindTexture(GL_TEXTURE_2D, m_FrameBufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0 , GL_RGB, width*scale, height*scale, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST ));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D ,m_FrameBufferTexture, 0);
    

    glGenRenderbuffers(1, &m_RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width*scale, height*scale);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);


    auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if(fboStatus != GL_FRAMEBUFFER_COMPLETE){
            std::cout << "ERROR FRAMEBUFFER " << fboStatus << std::endl;
        }
    
    m_Frame.Setup(2);
    for(unsigned long i = 0; i < ShaderVector.size(); i++){
        m_Frame.SetShader(ShaderVector.at(i).c_str());
    }
    m_Frame.FinishShader();
    m_Frame.Create2dQuad(0.0f,0.0f,0.0f, 0.0f,0.0f,0.0f, 2.0f,2.0f, 1.0f, 0.0f,0.0f,1.0f,1.0f, 5.0f);
    m_Frame.SetTexture(0, "u_Texture");
    m_Frame.SetFloatUniform("u_Size.height", m_Height);
    m_Frame.SetFloatUniform("u_Size.width", m_Width);
    
}

void FrameBufferObject::Update(int width, int height, float scale){
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER,  m_FBO));

        // The Texture and render buffer are basicaly remade every frame so the resolution can change
        GLCall(glBindTexture(GL_TEXTURE_2D, m_FrameBufferTexture));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0 , GL_RGB, width*scale, height*scale, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST ));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D ,m_FrameBufferTexture, 0));


        GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RBO));
        GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width*scale, height*scale));




        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        GLCall(glEnable(GL_DEPTH_TEST));
}

void FrameBufferObject::Render(){
    glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, FrameBuffTexture);
    glBindTexture(GL_TEXTURE_2D, this->m_FrameBufferTexture);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    GLCall(glDisable(GL_DEPTH_TEST)); 
    this->m_Frame.BindBufferData();
    this->m_Frame.Paint();
}