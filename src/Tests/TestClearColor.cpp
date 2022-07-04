#include "TestClearColor.h"


namespace test{
    TestClearColor::TestClearColor() : m_Color{1.0f, 0.75f, 0.75f, 1.0f}, m_running(true), m_Effect(false){

    }

    TestClearColor::~TestClearColor(){

    }

    void TestClearColor::OnUpdate(float deltaTime){

    }

    void TestClearColor::OnRender(int Width, int Height, float ScaleFactor){

        if(m_Effect){
            GLCall(glClearColor(m_Color[0], m_Color[1], m_Color[2], m_Color[3]));
            GLCall(glClear(GL_COLOR_BUFFER_BIT));
        }

    }

    void TestClearColor::OnImGui(){
        ImGui::Begin("Clear Color Test", &m_running);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("This test sets the background color of the screen");
            ImGui::Checkbox("Toggle live effect", &m_Effect);
            ImGui::ColorEdit4("Clear Color", (float*)&m_Color);
            if (ImGui::Button("Close Me"))
                m_running = false;
            ImGui::End();

        
    }

}