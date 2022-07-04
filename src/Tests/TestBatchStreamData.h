#pragma once
#include "test.h"
#include <array>



struct Vertex2DPos{
    float X;
    float Y;
    float Z;
};

struct Vertex2DTexCord{
    float X;
    float Y;
};

struct Vertex2D{
                Vertex2DPos Pos;
                Vertex2DTexCord TexCord;
                float TexID;
             };


namespace test{
  
    
    class TestBatchStreamData : public Test{

        private:
            GLuint Box1, Box2;
            bool m_Effect;
            glm::vec3 m_pos;

            int m_Width, m_Height;

            float m_QuadPos2[3] = {200.0f,210.0f,200.0f};

            glm::mat4 m_Proj, m_View;

            std::unique_ptr<VertexArray> m_VAO;
            std::unique_ptr<Shader> m_Shader;
            std::unique_ptr<IndexBuffer> m_IBO;
            std::unique_ptr<Texture> m_Texture;
            std::unique_ptr<VertexBuffer> m_VertexBuffer;

        public:
             

           
            bool m_running;
            TestBatchStreamData();
            ~TestBatchStreamData();


            Vertex2D *CreateQuad(Vertex2D *target, float X, float Y, float size, float TextureID);

            static void KeyCallBack( GLFWwindow *window, int key, int scancode, int action, int mods);

            void OnUpdate(float deltaTime) override;
            void Input(GLFWwindow *window, int key, int scancode, int action, int mods) override;
            void OnRender(int Width, int Height, float ScaleFactor) override;
            void OnImGui() override;


    };
}