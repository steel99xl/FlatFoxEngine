#pragma once

#include "../Renderer.h"

namespace test{

    class Test{
        private:




        public:
            Test(){}
            virtual ~Test(){}

            virtual void OnUpdate(float deltaTime){}
            virtual void Input(GLFWwindow *window, int key, int scancode, int action, int mods) {}
            virtual void OnRender(int Width, int Height, float ScaleFactor) {}
            virtual void OnImGui(){}



    };

    class Test_Menu : public Test{
        private:
            GLFWwindow *m_Window;
            Test*& m_CurrentTest;
            std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;



        public:

            Test_Menu(Test*& currentTestPointer, GLFWwindow *window);

            void OnImGui() override;

            template<typename T>
            void RegisterTest(const std::string &name){
                std::cout << "Registering test " << name << std::endl;
                m_Tests.push_back(std::make_pair(name,[](){return new T(); }));
            }


    };  



}