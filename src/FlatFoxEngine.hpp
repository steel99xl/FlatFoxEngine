#pragma once

#include "Imports/glew/include/GL/glew.h"
#include "Imports/glfw/include/GLFW/glfw3.h"
#include <iostream>
#include <cstring>
#include <functional>
#include <unordered_set>
#include <memory>
#include <vector>
#include <array>
#include <thread>
#include "FlatFoxRenderWarnings.hpp"
#include "Imports/glm/glm.hpp"
#include "Imports/glm/gtc/matrix_transform.hpp"
#include "Imports/stb_image/stb_image.h"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "Texture.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "SimpleObject.hpp"
#include "SimpleWorld.hpp"
#include "SimplePhysics.hpp"
#include "SimplePhysicsObjects.hpp"
#include "Camera.hpp"
#include "FrameBufferObject.hpp"
#include "FlatFoxWindow.hpp"


namespace FlatFoxEngine{
    class Engine{
        protected:
        FlatFoxWorld::SimpleWorld *m_World;
        FlatFoxWindow::Window *m_Window;

        public:

            inline Engine(){};
            inline ~Engine(){};

            //This function will house the internal managment for the physics thread (Physics engine provided by m_World)
            inline void PhysicsThreadManagment(){};

            inline void Setup(){};

            // Does the things
            inline void Run(){
                while(m_Window->IsOpen()){
                    m_Window->Update();

                    m_World->SetKeyArray(m_Window->GetKeyArray());
                    m_World->SetMouseButtonArray(m_Window->GetMouseButtonArray());
                    m_World->SetMousePos(m_Window->GetMousePos());

                    m_World->TheRealManagedUpdate();

                    m_Window->SetSeenRender();
                    m_World->ManagedWorldRender();


                    m_Window->SetPosFXRender();
                    m_World->PaintRenderFrame();

                    
                    m_Window->SwapRenderBuffer();
                }
            }
    };
}