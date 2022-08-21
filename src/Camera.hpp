#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "SimplePhysics.hpp"

 enum CamDirection{UP, DOWN, LEFT, RIGHT, FORWARD, BACK};


class Camera2D{
    protected:
    float m_LayerDepth, m_ScaleFactor, m_Width, m_Height;
    glm::vec3  m_pos;
    // Camera is at (4,3,3), in World Space // and looks at the  //glm::vec3(0,1,0)
    glm::mat4 m_Proj, m_View; // This MVP stuff has the same naems


    public:

    Camera2D();
    ~Camera2D();

    void Setup(glm::vec3 Pos, float ScaleFactor, float Width, float Height, float Layers);

    void Update(glm::vec3 Pos, float ScaleFactor = 0.0f, float Width = 0.0f, float Height = 0.0f, float Layers = 0.0f);
    void Input();
    inline glm::mat4 GetProj() {return m_Proj;};
    inline glm::mat4 GetView() {return m_View;};


};


class Camera3D{
    protected:
     // How far you can see from the camera
    float m_ViewDistance;
    float m_FOV;
    float m_AspecRatio;
    float m_DeltaTime;
    //Camera sensativity
    float m_Hsensitivity;
    float m_Vsensitivity;

    // 1 is "normal" and -1 is inverted
    int m_Yinvert = 1;
    int m_Xinvert = 1;

    //Camera  Position, Faceing, Rotation
    glm::vec3  m_pos, m_look, m_rotation;
    glm::mat4 m_Proj, m_View; // This MVP stuff has the same naems
    glm::vec2 m_WalkingDir; // same as look but just for the X and Z cordinates

    // Used where the camera is looking
    float m_Lyaw, m_Lpitch, m_LlastinputX, m_LlastinputY;


    public:

    Camera3D();
    ~Camera3D();

    void Setup(glm::vec3 Pos, glm::vec3 Look, glm::vec3 Angle, float Sensitivity, float Ratio, float FOV, float ViewDistance);

    void Update(float DeltaTime, float Ratio = 0.0f, float FOV = 0.0f, float ViewDistance = 0.0f);
    void Input();
    void Move(CamDirection direction, float Speed);
    FlatFoxPhysics::ForceDirection MoveDir(CamDirection direction, float Speed);
    void LookRelative(double xpos, double ypos);
    // Looks at a specific point in the world
    inline void LookAbsolute(float xpos, float ypos, float zpos){m_look[0] = xpos; m_look[1] = ypos; m_look[2] = zpos;};


    inline glm::mat4 GetProj() {return m_Proj;};
    inline glm::mat4 GetView() {return m_View;};
    inline glm::vec3 GetCurrentPos() {return m_pos;};
    inline void SetPos(float X, float Y, float Z){m_pos[0] = X; m_pos[1] = Y; m_pos[2] = Z;}
    inline glm::vec3 GetCurrentLook() {return m_look;};
    inline glm::vec3 GetCurrentRotation() {return m_rotation;};
    inline float GetCurrentFOV(){return m_FOV;};
    inline void InvertVertical(){m_Yinvert = -1;};
    inline void InvertHorizontal(){m_Xinvert = -1;};
    inline void UnInvertVertical(){m_Yinvert = 1;};
    inline void UnInvertHorizontal(){m_Xinvert = 1;};

    inline void SetHorizontalSensitivity(float Sensitivity){m_Hsensitivity = Sensitivity;};
    inline void SetVerticalSensitivity(float Sensitivity){m_Vsensitivity = Sensitivity;};

};

