//
// Created by steel99xl on 1/2/22.
//
#pragma once
#include "SimplePhysics.hpp"


// Some basic physics objects useing the SimplePhysicsObject struct

struct SimplePhysicsSphereObject : public FlatFoxPhysics::SimplePhysicsObject{

protected:
    float m_Radius;
    float m_Weight;

public:

    explicit SimplePhysicsSphereObject(std::string UniqueID = "NULL", std::string TypeID = "NULL",  unsigned long RenderObjectVectorPos = 0, bool IsPlayer = false, const std::vector<unsigned int> *PhysicsTypeReactionList = nullptr) : SimplePhysicsObject(UniqueID, TypeID, RenderObjectVectorPos, IsPlayer,PhysicsTypeReactionList) {
        // Junk number that hopfully wont conflict with other peoples
        this->TYPE = 42069;
    };


    void OnUpdate() override{
        std::cout << "uwu Sphere Update" << std::endl;
    };

};

struct SimplePhysicsBoxObject : public FlatFoxPhysics::SimplePhysicsObject{
protected:
    // C Should be used as the middle of the cube, or you can just use 4 points
    FlatFoxPhysics::PhysicsPoint A, B, C, D, E;
public:

    explicit SimplePhysicsBoxObject(std::string UniqueID = "NULL", std::string TypeID = "NULL",  unsigned long RenderObjectVectorPos = 0, bool IsPlayer = false, const std::vector<unsigned int> *PhysicsTypeReactionList = nullptr) : SimplePhysicsObject(UniqueID, TypeID, RenderObjectVectorPos, IsPlayer,PhysicsTypeReactionList){
        // Junk number to identify type
        this->TYPE = 69420;
    };

    void OnUpdate() override{
        std::cout << "uwu Box Update" << std::endl;
    };
};
