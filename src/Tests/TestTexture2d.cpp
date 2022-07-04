#include "TestTexture2d.h"


namespace test{
    TestTexture2d::TestTexture2d() : m_running(false), m_Effect(false), m_pos(0,0,0){
        float poss[16] = {
                    // set position values for vertexes (more info can be added)
                    0.0f, 0.0f,     0.0f, 0.0f,
                    200.0f, 0.0f,   1.0f, 0.0f,
                    200.0f, 200.0f, 1.0f, 1.0f,
                    0.0f, 200.0f,   0.0f, 1.0f
                     };

        unsigned int SqIndex[6] ={
                //tells what points to draw in what order
                0,1,2, 2,3,0
                };


        m_VAO = std::make_unique<VertexArray>();
        m_IBO = std::make_unique<IndexBuffer>(SqIndex,sizeof(SqIndex));
        m_Shader = std::make_unique<Shader>("assets/Shaders/ColorImg.shader");
        m_Texture = std::make_unique<Texture>("assets/Textures/OtherBox.png");

        //m_VertexBuffer = std::make_unique<VertexBuffer>(poss, 4* 4 * sizeof(float));
        m_VertexBuffer = std::make_unique<VertexBuffer>(poss, sizeof(poss));


        VertexBufferLayout layout;
        layout.Push(2,"float");
        layout.Push(2,"float");
        m_VAO->AddBuffer(*m_VertexBuffer,layout);    


        m_Shader->Bind();

        m_Shader->SetUniform1i("u_Texture", 0);

        m_View = glm::translate(glm::mat4(1.0f),glm::vec3(0,0,0));

        m_Color = ImVec4(0.0f, 0.55f, 0.6f, 1.00f);

    }

    TestTexture2d::~TestTexture2d(){

    }

    void TestTexture2d::OnUpdate(float deltaTime){

    }

    void TestTexture2d::OnRender(int Width, int Height, float ScaleFactor){

        m_Proj = glm::ortho(0.0f, (float)Width*ScaleFactor, 0.0f, (float)Height*ScaleFactor, -1.0f, 1.0f);


        Renderer renderer;
        m_Texture->Bind();

        glm::mat4 modle = glm::translate(glm::mat4(1.0f), m_pos);
        glm::mat4 mvp = m_Proj * m_View * modle;

        m_Shader->SetUniformMat4f("u_MVP", mvp);

        m_Shader->SetUniform4f("u_Color", m_Color.x, m_Color.y, m_Color.z, m_Color.w);

        if(m_Effect){
            GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
            GLCall(glClear(GL_COLOR_BUFFER_BIT));
        }

        renderer.Draw(*m_VAO, *m_IBO, *m_Shader);

    }

    void TestTexture2d::OnImGui(){
        ImGui::Begin("Texture Test ");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("This test sets the background color of the screen");
            ImGui::SliderFloat3("Position", &m_pos.x , 0.0f, 600.0f);
            ImGui::ColorEdit3("Line Color", (float*)&m_Color);
            ImGui::Checkbox("Clear Background Color", &m_Effect);
            ImGui::End();

        
    }

}