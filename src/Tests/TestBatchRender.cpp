#include "TestBatchRender.h"


namespace test{
    TestBatchRender::TestBatchRender() : m_running(false), m_Effect(false), m_pos(0,0,0){
        float poss[] = {
                    // set position values for vertexes (more info can be added)
                     0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                     200.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                     200.0f, 200.0f, 1.0f, 1.0f, 0.0f,
                     0.0f, 200.0f, 0.0f, 1.0f, 0.0f,
                    
                    // Second square with texter map data
                     210.0f, 210.0f, 0.0f, 0.0f, 1.0f,
                     410.0f, 210.0f, 1.0f, 0.0f, 1.0f,
                     410.0f, 410.0f, 1.0f, 1.0f, 1.0f,
                     210.0f, 410.0f, 0.0f, 1.0f, 1.0f
                     };

        unsigned int SqIndex[12] ={
                //tells what points to draw in what order
                0,1,2,
                2,3,0,
                
                // Draws second square
                4,5,6,
                6,7,4
                };


        //Take info and put it in a vertex
        m_VAO = std::make_unique<VertexArray>();

        m_IBO = std::make_unique<IndexBuffer>(SqIndex,sizeof(SqIndex));
        m_Shader = std::make_unique<Shader>("assets/Shaders/MultiImg.shader");
        m_Texture = std::make_unique<Texture>("assets/Textures/Box.png");


        Box1 = m_Texture->LoadTexture("assets/Textures/Box.png");
        Box2 = m_Texture->LoadTexture("assets/Textures/Logo.jpeg");

        //std::cout << "bound texture 1" << std::endl;
        GLCall(glActiveTexture(GL_TEXTURE0 + 0));
        GLCall(glBindTexture(GL_TEXTURE_2D, Box1));

        //std::cout << "bound texture 2 " << std::endl;
        GLCall(glActiveTexture(GL_TEXTURE0 + 1));
        GLCall(glBindTexture(GL_TEXTURE_2D, Box2));

        //VertexArray va;

        // The second optionhas to be the total difrent points not the amount...
        //m_VertexBuffer = std::make_unique<VertexBuffer>(poss, m_IBO->Unquie() * 5 * sizeof(float));
        m_VertexBuffer = std::make_unique<VertexBuffer>(poss, sizeof(poss));



        //std::cout << m_IBO->Unquie() << std::endl;


        VertexBufferLayout layout;
        // This defines the layout 
        layout.Push(2,"float");
        layout.Push(2,"float");
        layout.Push(1,"float");
        //layout.Push(1,"float");
        m_VAO->AddBuffer(*m_VertexBuffer,layout);    

        //std::cout << "Seting shader program" << std::endl;

        //IndexBuffer ibo(SqIndex,6);
        //GLCall(glUseProgram(m_Shader->GetRenderID()));

        m_Shader->Bind();

        std::cout << "Shader set" << std::endl;

        //auto loc = m_Shader->GetUniformLocation("u_Textures");
        int samplers[2] = {0 ,1};
        //GLsizei size = 2;
        //GLCall(glUniform1iv(loc, size, samplers));
        m_Shader->SetUniform1iv("u_Textures", 2, samplers);

        //Shader shader("assets/Shaders/ImgLoad.shader");
        //m_Shader->Bind();

     // This has to match the bound texture buffer

        //m_Shader->SetUniform1i("u_Texture", 0);

        

        m_View = glm::translate(glm::mat4(1.0f),glm::vec3(0,0,0));

    }

    TestBatchRender::~TestBatchRender(){

    }

    void TestBatchRender::OnUpdate(float deltaTime){

    }

    void TestBatchRender::OnRender(int Width, int Height, float ScaleFactor){

        Renderer renderer;

        

        //m_Shader->Bind();
        //GLCall(glUseProgram(m_Shader->GetRenderID()));

        m_Proj = glm::ortho(0.0f, (float)Width*ScaleFactor, 0.0f, (float)Height*ScaleFactor, -1.0f, 1.0f);


        glm::mat4 modle = glm::translate(glm::mat4(1.0f), m_pos);
        glm::mat4 mvp = m_Proj * m_View * modle;


        //auto loc = m_Shader->GetUniformLocation("u_MVP");
        //GLCall(glUniformMatrix4fv(loc, 1, GL_FALSE, &mvp[0][0]));

        m_Shader->SetUniformMat4f("u_MVP", mvp);

        if(m_Effect){
            GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
            GLCall(glClear(GL_COLOR_BUFFER_BIT));
        }

        renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
        //m_VAO->Bind();
        //m_IBO->Bind();

        //GLCall(glDrawElements(GL_TRIANGLES, m_IBO->GetCount(), GL_UNSIGNED_INT, nullptr));


    }

    void TestBatchRender::OnImGui(){
        ImGui::Begin("Batch Render");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("This test sets the background color of the screen");
            ImGui::SliderFloat3("Position", &m_pos.x , 0.0f, 600.0f);
            ImGui::Checkbox("Toggle live effect", &m_Effect);
            ImGui::End();

        
    }

}