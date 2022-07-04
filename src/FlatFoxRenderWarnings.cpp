//
// Created by Steel LaPointe on 3/25/22.
//
#include "FlatFoxRenderWarnings.hpp"
void GLClearError(){
    while(glGetError() != GL_NO_ERROR);
}

void GLLogCall(const char *function, const char *file, int line){
    while(GLenum error = glGetError()){
        std::cout << "[OpenGL Error] "<< error << " "<< function << " " << file << " " << line <<  std::endl;
        //std::cout << gluErrorString(error) << std::endl;
    }
}
