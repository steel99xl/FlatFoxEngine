#pragma once
#include "test.h"

namespace test{
    
    class TestClearColor : public Test{

        private:
            float m_Color[4];
            bool m_Effect;

        public:
            bool m_running;
            TestClearColor();
            ~TestClearColor();


            void OnUpdate(float deltaTime) override;
            void OnRender(int Width, int Height, float ScaleFactor) override;
            void OnImGui() override;


    };
}