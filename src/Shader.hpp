#pragma once

#include <regex>
#include <fstream>
#include <cstring>
#include <sstream>
#include <unordered_map>
#include <vector>
#include "Imports/glew/include/GL/glew.h"
#include "FlatFoxRenderWarnings.hpp"
#include "Imports/glm/glm.hpp"
#include "Imports/glm/gtc/matrix_transform.hpp"
//#include "FlatFoxGL.hpp"

enum ShaderType{
    NONE = -1, VERTEX = 0, FRAGMENT = 1, GEOMETRY = 2
};

struct ShaderProgramSource{
    ShaderType type;
    std::string shaderSource;

};

class Shader{
protected:
    std::string m_FilePath;
    unsigned int m_RenderID;
    unsigned int m_TempRender;
    mutable std::unordered_map<std::string, int> m_UniformLocationCache;

    std::vector<unsigned int> CompiledShaders;

public:
    Shader();
    ~Shader();

    void SetShader(const std::string &filePath, int TotalLights = 0);

    void Finish();

    void ClearCompiledShaders();

    void Bind() const;
    void UnBind();

    // Set Uniforms
    void SetUniform1i(const std::string &name, int value);
    void SetUniform1iv(const std::string &name, GLsizei size, const GLint *array);
    void SetUniform1f(const std::string &name, float value);
    void SetUniform3f(const std::string &name, float v0, float v1, float v2);
    void SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3);
    void SetUniformMat4f(const std::string &name, const glm::mat4 &matrix);

    int GetUniformLocation(const std::string &name) const ;
    inline int GetRenderID() const {return m_RenderID;};

    inline std::vector<unsigned int> GetCompiledShaders(){return CompiledShaders;}
    inline void SetCompiledShaders(std::vector<unsigned int> CompShaders){ CompiledShaders = CompShaders;}

private:
    ShaderProgramSource ParseShader(int TotalLights = 4);


    void CreateShader(ShaderType &type, std::string &shaderSource);
    unsigned int CompileShader(unsigned int type, std::string &source);

};



