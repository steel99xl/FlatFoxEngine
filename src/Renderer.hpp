//
// Created by steel99xl on 6/14/21.
//

#pragma once

//#include "FlatFoxGL.hpp"

#include "Imports/glew/include/GL/glew.h"
#include <iostream>
#include <cstring>
#include <functional>
#include <unordered_set>
#include <memory>
#include <vector>
#include <array>
#include "FlatFoxRenderWarnings.hpp"
#include "Imports/glm/glm.hpp"
#include "Imports/glm/gtc/matrix_transform.hpp"
//#include "Imports/imgui/imgui.h"
//#include "Imports/imgui/imgui_impl_glfw.h"
//#include "Imports/imgui/imgui_impl_opengl3.h"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"






// Get error event from gpu

class Renderer{

protected:

public:

    Renderer();
    ~Renderer();

    void Clear() const;

    void Draw(class VertexArray &va, class IndexBuffer &ib, class Shader &shader);

};

