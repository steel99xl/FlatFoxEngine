#pragma once
#include "../FlatFoxEngine.hpp"

    
class TestWorld{

    private:
        float m_DeltaTime, m_PastTime, m_PhysicsDeltaTime, m_PhysicsPastTime;
        GLuint Tex1, Tex2, Tex3;
        bool m_Effect;
        glm::vec3 m_pos2D;
        float m_FOV;

        glm::vec3 m_3dCamPos, m_3dCamPosPrevious;

        glm::mat4 m_Projection, m_View;

        //ImVec4 m_Color, m_Color2, m_Color3;
        glm::vec4 m_Color = glm::vec4(0.0f,0.0f,1.0f,1.0f);

        //float m_yaw, m_pitch, m_lastX, m_lastY;
        // These are stored as floats so the rounding can be ajusted
        float m_Width, m_Height, m_Scale;

        FrameBufferObject m_FBO;
        FlatFoxObject::SimpleObject Frame;

        double m_MouseX = 0.0, m_MouseY = 0.0;

        // This is currently hnit being used
        unsigned int ShadowMapFBO, ShadowMapRBO,ShadowMapTexture;
        unsigned int ShadowWidth = 2048;
        unsigned int ShadowHeight = 2048;
        //


        FlatFoxObject::SimpleObject PlayerBlock;
        FlatFoxObject::SimpleObject TealBlock;
        // Lol Voxl Land Test set back to about 5k when done
        FlatFoxObject::SimpleObject Land;
        FlatFoxObject::SimpleObject Platform;
        FlatFoxObject::SimpleObject OtherSuns;
        FlatFoxObject::SimpleObject Sun;


        FlatFoxPhysics::PhysicsEngine BasicPhysics = FlatFoxPhysics::PhysicsEngine(2.0f ,0.0f,-1.0f,0.0f);


        //Elements for Physics Engine
        std::string m_PlayerObjectID = "PlayerObject";
        std::string m_Box = "Box";
        // This is going to also be the 'walls' for now
        std::string m_Gound = "Gound";
        std::vector<FlatFoxPhysics::ForceDirection> m_NewPlayerDirection;
        std::vector<FlatFoxPhysics::ColisionInfo> m_ObjectColissions;
        std::vector<FlatFoxPhysics::QuadPhysicsBody> PlayerPhysics, LandPhysics, TealBlockPhysics;

        // The TealBlock on the new physics Engine
        SimplePhysicsSphereObject BasePlayerandPushableObject;
        SimplePhysicsBoxObject BaseNonMovableobjects;
        // One object of each type must exist for any duplicates to exist;
        //SimplePhysicsSphereObject TBPhysics;
        //SimplePhysicsSphereObject PlayerPObject;
        // This is the Platform only being represented as the new objet
        //SimplePhysicsBoxObject PlatformPhysics;

        // Temp PlaceHolder for Object Posisons;
        std::vector<FlatFoxPhysics::PhysicsPos> m_ObjectWorldPositions;



        float PlayerMovmentSpeed;

        FlatFoxObject::SimpleMaterialInfo BasicMetalCube;


        Camera2D SimpleCam;
        //SimpleObject Object;// = SimpleObject(1000);
        Camera3D AdvancedCam;

    public:

        bool m_running;
        TestWorld();
        ~TestWorld();


        void Setup();

        void PhysicsUpdate(float MaxUpdateSpeed);

        void OnUpdate(float deltaTime, float width, float hight, float scale = 1.0f);
        void KeyInput(std::array<int, 512> Keys);
        void MouseButton(std::array<int, 32> MouseButtons);
        void MouseInput(double xpos, double ypos);
        // Yes this needs to be called every frame if you have moving objects;
        void GenShadows();
        void OnRender();
        void PaintFrame();
        void OnGui();

        inline void Start() { m_running = true; };
        inline bool IsWorldRunning() { return m_running; };
        inline void Stop() { m_running = false; };
        inline void Test(double xpos, double ypos){std::cout << xpos << " | " << ypos << std::endl;};


    };
