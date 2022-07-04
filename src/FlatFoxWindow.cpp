#include "FlatFoxWindow.hpp"

using namespace FlatFoxWindow;

Window::Window(int width, int height, std::string title, float OSResolutionScale ,int OpenGLMajorVersion, int OpenGLMinorVersion){
    m_Width = width;
    m_Height = height;
    m_Title = title;
    m_OSScale = OSResolutionScale;
    m_ResolutionScale = 1.0f;

    if(!glfwInit()){
        std::cout << "Failed to initialize GLFW" << std::endl;
        exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OpenGLMajorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OpenGLMinorVersion);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


}

Window::~Window(){
    glfwDestroyWindow(m_Window);
}


void Window::Update(){
    m_CurrentFrameTime = glfwGetTime();
    m_DeltaTime = m_CurrentFrameTime - m_LastFrameTime;
    m_LastFrameTime = m_CurrentFrameTime;

    m_Keys = hexmap.GetKeyPress();

    m_MouseButtons = hexmap.GetMouseButtonPress();

    if(this->IsCursorLock()){
        m_MouseX = hexmap.GetMousePos()[0];
        m_MouseY = hexmap.GetMousePos()[1];
    }
    
    //Some base key press functionality provided by the window
    if(m_Keys[GLFW_KEY_ESCAPE] == GLFW_PRESS){
    this->ToggleCursorLock();
    this->CursorLock(this->IsCursorLock());
    // sleeps for 100 milliseconds cause this event should have a longer responce 
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    if(m_Keys[GLFW_KEY_LEFT_BRACKET] == GLFW_PRESS){
        this->SetResolutionScale(this->GetResolutionScale() - 0.02);
        if(this->GetResolutionScale() > 1.0){
            std::cout << "Warning Resolusion scale above 1.0" << " | " << this->GetResolutionScale() << std::endl;
        } else if(this->GetResolutionScale() < 1.0){
            std::cout << "Warning Resolusion scale bellow 1.0" << " | " << this->GetResolutionScale() << std::endl;
        }  
        if(this->GetResolutionScale() < 0.01){
            this->SetResolutionScale(0.01);
        }
    }

    if(m_Keys[GLFW_KEY_RIGHT_BRACKET] == GLFW_PRESS){
        this->SetResolutionScale(this->GetResolutionScale() + 0.02);
        if(this->GetResolutionScale() > 1.0){
            std::cout << "Warning Resolusion scale above 1.0" << " | " << this->GetResolutionScale() << std::endl;
        }else if(this->GetResolutionScale() < 1.0){
            std::cout << "Warning Resolusion scale bellow 1.0" << " | " << this->GetResolutionScale() << std::endl;
        } 
        if(this->GetResolutionScale() > 2.00){
            this->SetResolutionScale(2.00);
        }
    }

    if(m_Keys[GLFW_KEY_V] == GLFW_PRESS){
        this->ToggleAttemptMaxFrameRateTarget();
        if(this->IsAttemptMaxFrameRateTarget()){
            std::cout << "VSync : ON" << std::endl;
            this->ManageVSync(this->IsAttemptMaxFrameRateTarget());
        } else {
            std::cout << "VSync : OFF" << std::endl;
            this->ManageVSync(this->IsAttemptMaxFrameRateTarget());
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    glfwPollEvents();
    glfwSetWindowTitle(m_Window, m_Title.c_str());
    glfwGetWindowSize(m_Window, &m_Width, &m_Height);

    if(m_AtmpMaxFPS){
        Window::WindowThreadSleep();
    }

}

void Window::SetSeenRender(){
    glViewport(0, 0, Window::GetScaledWidth(), Window::GetScaledHeight());
}

void Window::SetPosFXRender(){
    glViewport(0, 0, m_Width * m_OSScale, m_Height * m_OSScale);
}

void Window::SwapRenderBuffer(){
    glfwSwapBuffers(m_Window);
}

void Window::SetResolutionScale(float scale){
    m_ResolutionScale = scale;
}

void Window::ManageVSync(bool enable){
    if(enable){
        glfwSwapInterval(1);
    }else{
        glfwSwapInterval(0);
    }
}

void Window::SetMouseButtonArray(int button){
    m_MouseButtons[button] = glfwGetMouseButton(m_Window, button);
}

void Window::CursorLock(bool enable){
    if(enable){
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPos(m_Window, m_LastMouseX, m_LastMouseY);
    }else{
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void Window::KeyInputCallBack(GLFWwindow *InputWindow, int key, int scancode, int action, int mods){
    m_Keys[key] = glfwGetKey(InputWindow, key);
}


void Window::Init(){
    std::cout << "Initializing Window" << std::endl;
    m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), NULL, NULL);
    if(!m_Window){
        glfwTerminate();
        exit(-1);
    }
    std::cout << "Setting Windoow Context" << std::endl;
    glfwMakeContextCurrent(m_Window);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    if(glewInit() != GLEW_OK){
        std::cout << "Failed to initialize GLEW" << std::endl;
        exit(-1);
    }

    glEnable(GL_CULL_FACE);

    //glBindVertexArray(0);
    //glUseProgram(0);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    

    glfwSetKeyCallback(m_Window, &Input::KeyboardCallback);
    glfwSetCursorPosCallback(m_Window, &Input::MousePosCallback);
    glfwSetMouseButtonCallback(m_Window, &Input::MouseButtonCallback);
    glfwSetInputMode(m_Window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    
}

bool Window::IsOpen(){
    return !glfwWindowShouldClose(m_Window);
}