#include "Shader.hpp"


Shader::Shader() : m_RenderID(0){

    m_TempRender = glCreateProgram();

}

Shader::~Shader(){

    GLCall(glDeleteProgram(m_RenderID));

}

void Shader::SetShader(const std::string &filePath){
    m_FilePath = filePath;
    ShaderProgramSource source = ParseShader();
    //std::cout << "Test" << std::endl;
    //std::cout << source.shaderSource << std::endl;
    //std::cout << "End of Test" << std::endl;
    CreateShader(source.type, source.shaderSource);

}

void Shader::Finish(){
    // DELTE COMPILED SHADER PROGRAMS
    for(long unsigned int i = 0; i < CompiledShaders.size(); i++){
        glAttachShader(m_TempRender, CompiledShaders[i]);
    }

    glLinkProgram(m_TempRender);
    glValidateProgram(m_TempRender);

    m_RenderID = m_TempRender;
    std::cout << m_RenderID << std::endl;
}


void Shader::ClearCompiledShaders(){
    for(long unsigned int i = 0; i < CompiledShaders.size(); i++){
        glDeleteShader(CompiledShaders[i]);
    }

    CompiledShaders.clear();
}

void Shader::Bind() const{

    GLCall(glUseProgram(m_RenderID));

}

void Shader::UnBind(){

    GLCall(glUseProgram(0));

}

void Shader::SetUniform1i(const std::string &name, int value){

    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1iv(const std::string &name, GLsizei size, const GLint *array){
    GLCall(glUniform1iv(GetUniformLocation(name), size, array));
}

void Shader::SetUniform1f(const std::string &name, float value){

    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform3f(const std::string &name, float v0, float v1, float v2){
    GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

void Shader::SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3){

    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));

}

void Shader::SetUniformMat4f(const std::string &name, const glm::mat4 &matrix){

    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));

}

int Shader::GetUniformLocation(const std::string &name) const {

    if(m_UniformLocationCache.find(name) != m_UniformLocationCache.end()){
        return m_UniformLocationCache[name];
    }

    GLCall(int location = glGetUniformLocation(m_RenderID, name.c_str()));
    if(location == -1){
        std::cout << "Error " << name << " is not being used or is not found..." << std::endl;
    }
    m_UniformLocationCache[name] =  location;
    return location;

}

ShaderProgramSource Shader::ParseShader(){
    std::ifstream File(m_FilePath);
    std::string lines;
    std::stringstream ss;

    //ss << "\n";

    ShaderType type = NONE;

    while(getline(File, lines)){
        if(lines.find("#Shader") != std::string::npos){
            if(lines.find("vertex") != std::string::npos){
                //set to vertex
                type = ShaderType::VERTEX;
                //std::cout << "vertex found" << std::endl;
            } else if(lines.find("fragment") != std::string::npos){
                //set to fragment
                type = ShaderType::FRAGMENT;
                //std::cout << "fragment found" << std::endl;
            } else if(lines.find("geometry") != std::string::npos){
                type = ShaderType::GEOMETRY;
            }
        }  else {
            ss << lines << "\n";
        //ss << lines << "\n";
        }
    }
    return {type, ss.str()};
}

void Shader::CreateShader(ShaderType &type, std::string &shaderSource){
    switch(type){
        case NONE:
            break;
        case VERTEX:
            std::cout << "Making Vertex Shader" << std::endl;
            CompiledShaders.push_back(CompileShader(GL_VERTEX_SHADER, shaderSource));
            break;
        case FRAGMENT:
            std::cout << "Making Fragment Shader" << std::endl;
            CompiledShaders.push_back(CompileShader(GL_FRAGMENT_SHADER, shaderSource));
            break;
        case GEOMETRY:
            CompiledShaders.push_back(CompileShader(GL_GEOMETRY_SHADER, shaderSource));
            break;
    }

    //unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    //unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
}

unsigned int Shader::CompileShader(unsigned int type, std::string &source){
    unsigned int id =  glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id,1,&src, nullptr);

    glCompileShader(id);

    //TODO : error checking
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE){
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH,&length);
        char *message = (char *)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout  << "Error " << message << std::endl;
        std::cout << source << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

