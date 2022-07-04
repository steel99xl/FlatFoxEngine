#include "test.h"

namespace test{

        Test_Menu::Test_Menu(Test*& currentTestPointer, GLFWwindow *window) : m_CurrentTest(currentTestPointer), m_Window(window){

        }

        void Test_Menu::OnImGui(){

            for(auto& test : m_Tests){
                if(ImGui::Button(test.first.c_str())){
                    m_CurrentTest = test.second();
                }
            }


        }




}

