#include "src/FlatFoxEngine.hpp"
#include "src/Seens/BasicMovement.hpp"

// Window Context
FlatFoxWindow::Window window(720, 480, "FlatFox");

// Worlds
TestWorld World;

//This is the Physics thread
void SecondThread(int UpdateSpeed){
    // This needs to be moved into the Physics Engine for a cleaner look
    float currentPhysicsFrame, lastPhysicsFrame;
    lastPhysicsFrame = 0.0f;
    // This is to wait for the renderinfg thread to startup
    //std::this_thread::sleep_for(std::chrono::milliseconds(200));
    auto WaitTime = std::chrono::milliseconds(UpdateSpeed);
    while(World.IsWorldRunning()){
        currentPhysicsFrame= glfwGetTime();
        auto StartTime = std::chrono::steady_clock::now();

        World.PhysicsUpdate(currentPhysicsFrame - lastPhysicsFrame);
        lastPhysicsFrame = currentPhysicsFrame; 

        auto EndTime = std::chrono::steady_clock::now();
        auto ElapsedTime = EndTime - StartTime;
        auto FinalTime = WaitTime - ElapsedTime;
        if(FinalTime > std::chrono::milliseconds::zero()){
            std::this_thread::sleep_for(FinalTime);
        }
    }
    
}


int main(){

    glfwSwapInterval(0);
    
    std::string TempTitle = "FlatFox Window";
    // just to test/show some options
    window.setWidth(720);
    window.setHeight(480);
    window.setOSScale(2.0f);
    window.SetResolutionScale(1.0f);
    window.SetMaxFrameRateTarget(70);
    window.ToggleAttemptMaxFrameRateTarget();

    window.Init();


    std::cout << glGetString(GL_VERSION) << std::endl;
    std::cout << "GL_SHADING_LANGUAGE_VERSION: " << glGetString (GL_SHADING_LANGUAGE_VERSION) << std::endl;


    World.Setup();
    //World.m_running = true;
    std::cout << "World Setup" << std::endl;
    std::thread PhysicsThread(SecondThread,12);
    PhysicsThread.detach();
    std::cout << "Physics Thread Running..." << std::endl;

    // Yes the KeyCallBack and MouseCallBack must be a function in main (or where ever you have the ability to call) or you can handle passing a static pointer from a class to it.
    /*
    glfwSetKeyCallback(window.GetWindow(), KeyCallBack);
    glfwSetCursorPosCallback(window.GetWindow(), MousePosCallBack);
    glfwSetMouseButtonCallback(window.GetWindow(), MouseButtonCallBack);
    glfwSetInputMode(window.GetWindow(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    */
    

    // Draw LOOP
    float FPS = 0;
    /* Loop until the user closes the window */
    std::cout << "Starting Update Loop..." << std::endl;
    while (window.IsOpen()) {

        //window.SetResolutionScale(ResolutionScale);
        World.KeyInput(window.GetKeyArray());
        if(window.IsCursorLock()){
            World.MouseInput(window.GetMouseX(), window.GetMouseY());
        }
        World.MouseButton(window.GetMouseButtonArray());
        
        window.Update();

        FPS = 1.0f / window.GetDeltaTime();
        std::string NewTile = TempTitle + "( " + std::to_string(FPS) + "FPS)";
        window.ChangeWindowTitle(NewTile.c_str());


        World.OnUpdate(window.GetDeltaTime(), (float) window.GetScaledWidth(), (float) window.GetScaledHeight());

        window.SetSeenRender();
        World.OnRender();

        window.SetPosFXRender();
        World.PaintFrame();

        window.SwapRenderBuffer();

    }

    World.Stop();
    //World.m_running = false;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    glfwTerminate();
    return 0;
}
