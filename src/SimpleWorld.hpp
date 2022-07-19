#pragma once
#include "SimplePhysics.hpp"
#include "Camera.hpp"

namespace FlatFoxWorld{

    class SimpleWorld{
        protected:
        // yes
        std::unique_ptr<FlatFoxPhysics::PhysicsEngine> m_PhysicsFox;
        FlatFoxObject::SimpleObject m_Land;

        // This is just so subsituting cams will be easier potentialy
        glm::mat4 m_Projection, m_View;
        // Each world must have 1 camera so this is it lol
        Camera3D  m_MainCamera;

        // Legacy Varaible but still used
        bool m_running;

        // This should be more than enough keys for any keyboard
        std::array<int,512> m_Keys;
        std::array<int,32> m_MouseButtons;
        std::array<double,2> m_MousePos;

        //The FlatFoxEngine Will take auto render the objects
        std::vector<std::unique_ptr<FlatFoxObject::SimpleObject> > m_Objects;


        public:

        SimpleWorld();
        ~SimpleWorld();

        virtual void Setup();

        //These 3 dont get any input cause their relative arrays get updated on Update()
        virtual void KeyInput();
        virtual void MouseClick();
        virtual void MouseMove();

        virtual void RenderWorld();
        virtual void PaintRenderFrame();


        inline void Start() { m_running = true; };
        inline bool IsWorldRunning() { return m_running; };
        inline void Stop() { m_running = false; };

        // These set the internal variable used by other functions
        inline void SetKeyArray(std::array<int,512> Keys){m_Keys = Keys;}
        inline void SetMouseButtonArray(std::array<int,32> MouseButtons){m_MouseButtons = MouseButtons;}
        inline void SetMousePos(std::array<double,2> MousePos){m_MousePos = MousePos;}

        inline std::vector<std::unique_ptr<FlatFoxObject::SimpleObject> >* GetWorldObjects(){return &m_Objects;}

        // These be defined at the bottom so it can use the full class....
        virtual void Update();

        // uwu the internal update thats not supose to be over writen by the end user
        inline void TheRealManagedUpdate(){
            this->KeyInput();
            this->MouseMove();
            this->MouseClick();

            this->Update();
            
            // The rendering will be called by the engine
            //this->RenderWorld();
            //this->PaintRenderFrame();

        };


        // This will allow for quick overhauls of the render pipeline
        inline void ManagedWorldRender(){
            this->RenderWorld();
        };


    };

}