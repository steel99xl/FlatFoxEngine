#pragma once

#include "Imports/glew/include/GL/glew.h"
#include "Imports/glfw/include/GLFW/glfw3.h"
#include <thread>
#include "Renderer.hpp"


namespace FlatFoxWindow{
class Input{
    private:
    std::array<int,512> m_Keys;
    std::array<double,2> m_MousePos;
    std::array<int,32> m_MouseButtons;
    Input(void) // private constructor necessary to allow only 1 instance
    {
    }

    Input(Input const&); // prevent copies
    void operator=(Input const&); // prevent assignments

    public:
    static Input& getInstance() // Singleton is accessed via getInstance()
    {
        static Input instance; // lazy singleton, instantiated on first use
        return instance;
    }

    static void KeyboardCallback(GLFWwindow *InputWindow, int key, int scancode, int action, int mods) // this method is specified as glfw callback
    {
        //here we access the instance via the singleton pattern and forward the callback to the instance method
        getInstance().KeyboardCallbackImpl(InputWindow ,key, scancode , action, mods); 
    }

     static void MousePosCallback(GLFWwindow *InputWindow, double xpos, double ypos) // this method is specified as glfw callback
    {
        //here we access the instance via the singleton pattern and forward the callback to the instance method
        getInstance().MousePosCallbackImpl(xpos,ypos); 
    }

    static void MouseButtonCallback(GLFWwindow *InputWindow,int button, int action, int mods){
        getInstance().MouseButtonCallbackImpl(InputWindow, button);
    }

    void KeyboardCallbackImpl(GLFWwindow *InputWindow, int key, int scancode, int action, int mods) //this is the actual implementation of the callback method
    {
        m_Keys[key] = glfwGetKey(InputWindow, key);
        
    }

    void MousePosCallbackImpl(double xpos, double ypos){
        m_MousePos[0] = xpos;
        m_MousePos[1] = ypos;
    }

    void MouseButtonCallbackImpl(GLFWwindow *InputWindow, int button){
        m_MouseButtons[button] = glfwGetKey(InputWindow, button);
    }

    
    
    std::array<int,512> GetKeyPress(){return m_Keys;}
    std::array<double,2> GetMousePos(){return m_MousePos;}
    std::array<int,32> GetMouseButtonPress(){return m_MouseButtons;}

};

class Window{
    protected:
        // ya great naming scheme...
        GLFWwindow* m_Window;
        Input &hexmap = Input::getInstance(); 
        int m_Width;
        int m_Height;
        std::string m_Title;
        // scale is determined by the OS, Mac is 2.0... unless on a non-retina display
        int m_OSScale;
        float m_ResolutionScale;

        // This should be more than enough keys for any keyboard
        std::array<int,512> m_Keys;

        // So far the most mouse buttons ive seen on a mouse is 19
        std::array<int, 32> m_MouseButtons;
        double m_MouseX, m_MouseY, m_LastMouseX, m_LastMouseY;

        bool m_CursorLock = false;

        float m_CurrentFrameTime, m_DeltaTime;
        float m_LastFrameTime = 0.0f;

        int m_MaxFrameRate;
        bool m_AtmpMaxFPS = false;

        inline void WindowThreadSleep() {std::this_thread::sleep_for(std::chrono::milliseconds(m_MaxFrameRate)); };


    public:
        Window(int width, int height, std::string title, float OSResolutionScale = 1.0f, int OpenGLMajorVersion = 3, int OpenGLMinorVersion = 3);
        ~Window();


        void Update();

        void SetSeenRender();

        void SetPosFXRender();

        void SwapRenderBuffer();

        void SetResolutionScale(float scale);

        inline float GetResolutionScale() { return m_ResolutionScale; };

        void ManageVSync(bool enabled);

        inline void SetMaxFrameRateTarget(int target) {m_MaxFrameRate = 1000/target;}

        inline void ToggleAttemptMaxFrameRateTarget() { m_AtmpMaxFPS = !m_AtmpMaxFPS; };

        inline bool IsAttemptMaxFrameRateTarget() { return m_AtmpMaxFPS; }

        inline void ChangeWindowTitle(std::string title){m_Title = title;}

        inline  std::string GetWindowTitle() {return m_Title;}

        inline float GetDeltaTime() {return m_DeltaTime;}

        inline GLFWwindow* GetWindow() {return m_Window;}

        inline std::array<int,512> GetKeyArray() {return m_Keys;}

        void SetMouseButtonArray(int button);

        inline std::array<int,32> GetMouseButtonArray() {return m_MouseButtons;}
        inline std::array<double,2> GetMousePos(){std::array<double,2> i = {m_MouseX, m_MouseY}; return i;}

        inline void SetMousePos(double x, double y) {m_LastMouseX = m_MouseX; m_LastMouseY = m_MouseY; m_MouseX = x; m_MouseY = y;}

        inline void SetLastMousePos(double x, double y) {m_LastMouseX = x; m_LastMouseY = y;}

        inline double GetMouseX(){return m_MouseX;}

        inline double GetMouseY(){return m_MouseY;}
        
        inline double GetLastMouseX() {return m_LastMouseX;}

        inline double GetLastMouseY() {return m_LastMouseY;}

        inline void ToggleCursorLock() {m_CursorLock = !m_CursorLock;}
        
        inline bool IsCursorLock() {return m_CursorLock;}

        void CursorLock(bool enabled);

        void KeyInputCallBack(GLFWwindow *InputWindow, int key, int scancode, int action, int mods);

        void Init();

        bool IsOpen();

        inline void setWidth(int width) {m_Width = width;}
        inline void setHeight(int height) {m_Height = height;}
        inline void setOSScale(float scale) {m_OSScale = scale;}

        inline int GetScaledWidth() const { return m_Width * m_ResolutionScale;}
        inline int GetScaledHeight() const { return m_Height * m_ResolutionScale;}


};
}