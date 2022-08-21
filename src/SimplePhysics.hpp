#pragma once
#include <cmath>
#include <utility>
#include <vector>
#include <thread>
#include <cstring>
#include "SimpleObject.hpp"
// THis is just for debuging
#include <iostream>
//#include "SimpleObject.hpp"
// This file should not really care about anything else in the engine, outside the data directly sent to it

namespace FlatFoxPhysics {

    enum PhysicsModle{Cube, Sphear, RenderModle};

    struct ForceDirection {
        bool IsBlank;
        float X;
        float Y;
        float Z;
        float Speed;

        ForceDirection(bool IsDummy = false) {
            if(IsDummy){
                IsBlank = true;
            }
        }

        ~ForceDirection() {}

        operator bool() {return IsBlank;}

        void Input(float x, float y, float z, float speed) {
            X = x;
            Y = y;
            Z = z;
            Speed = speed;
            IsBlank = false;
        }


    };

// Yes this is basicaly the same as the ForceDirection But with a diffrent name
    struct PhysicsPos {
        float X;
        float Y;
        float Z;

        PhysicsPos() {}

        ~PhysicsPos() {}
        void VertexPosInput(FlatFoxObject::VertexPos VertexPos){
            X = VertexPos.X;
            Y = VertexPos.Y;
            Z = VertexPos.Z;
        }
        void Input(float x, float y, float z) {
            X = x;
            Y = y;
            Z = z;
        }
    };

    struct PhysicsPoint {
        PhysicsPos Pos;
        float Weight;
        //ForceDirection Movment;
        void Input(PhysicsPos Position, float TotalWeight){
           Pos = Position;
           Weight = TotalWeight;
        }
    };

    struct PhysicsLine {
        PhysicsPoint PosA;
        PhysicsPoint PosB;
        float Diff;
        ForceDirection Normal; // This is just going to be stored here as a referance

    };


    struct QuadPhysicsBody {
        PhysicsPoint PosA;
        PhysicsPoint PosB;
        PhysicsPoint PosC;
        PhysicsPoint PosD;
        ForceDirection PlaneNorm;

    };

    struct TriPhysicsBody {
        PhysicsPoint PosA;
        PhysicsPoint PosB;
        PhysicsPoint PosC;
        ForceDirection PlaneNorm;

    };

    struct SphearPhysicsBody {
        PhysicsPoint PosA;
        float Radius;
    };


    struct PlaneMinMax {
        ForceDirection Min;
        ForceDirection Max;
        ForceDirection Normal;
    };


    struct ColisionInfo {
        bool IsColision; // Simple "yes" "no" colision
        ForceDirection MovmentDirectionA; //This can be calculated by comparing the dirs of the 2 coliding object
        ForceDirection MovmentDirectionB;
        ForceDirection PastMovmentDirectionA;
        ForceDirection PastMovmentDirectionB;

        ForceDirection CurentMovmentDirectionA;
        ForceDirection CurentMovmentDirectionB;
        float Force; // the Force that should applied to the colistion objectA

        operator bool() { return IsColision; }
    };



    inline PhysicsPos MovePhysicsObject(PhysicsPos Object, ForceDirection NormalDir, float DeltaTime, float Speed = -1.0f){
        Speed = Speed * DeltaTime;

        if(Speed != -1.0f) {
            NormalDir.X = NormalDir.X * Speed;
            NormalDir.Y = NormalDir.Y * Speed;
            NormalDir.Z = NormalDir.Z * Speed;
        }

        Object.X += NormalDir.X;
        Object.Y += NormalDir.Y;
        Object.Z += NormalDir.Z;

        return Object;
    }


    inline ForceDirection NormalizeVectorOfForceDirection(std::vector<ForceDirection> VectorOfForces){
        ForceDirection Output;
        // Speed is now used when normalizing the vector
        int VectorSize = VectorOfForces.size();
        float Temp;
        Output.X = 0.0f;
        Output.Y = 0.0f;
        Output.Z = 0.0f;

        if(!(VectorSize == 0)){
            for(int i = 0; i < VectorSize; i ++){
                Output.X += VectorOfForces[i].X * VectorOfForces[i].Speed;
                Output.Y += VectorOfForces[i].Y * VectorOfForces[i].Speed;
                Output.Z += VectorOfForces[i].Z * VectorOfForces[i].Speed;
            }

            Temp = std::sqrt(Output.X * Output.X + Output.Y * Output.Y + Output.Z * Output.Z);


            if(Temp != 0.){
                Output.X /= Temp;
                Output.Y /= Temp;
                Output.Z /= Temp;
            }

            Output.Speed = Temp;

        }


        return Output;
    };

    struct MultiThreadPhysUpdate {


    };

    inline std::vector<QuadPhysicsBody>MakePhysicsQuads(std::vector<PhysicsPos> Pos, std::vector<PhysicsPos> Normal, std::vector<float> Weights){
        std::vector<QuadPhysicsBody> Output;

        QuadPhysicsBody TempQuad;
        PhysicsPoint PointA;
        PhysicsPoint PointB;
        PhysicsPoint PointC;
        PhysicsPoint PointD;

        for(long unsigned int i = 0; i < Pos.size(); i+=4){
            PointA.Pos.Input(Pos[i].X, Pos[i].Y, Pos[i].Z);
            PointA.Weight = Weights[i];

            PointB.Pos.Input(Pos[i+1].X, Pos[i+1].Y, Pos[i+1].Z);
            PointB.Weight = Weights[i+1];


            PointC.Pos.Input(Pos[i+2].X, Pos[i+2].Y, Pos[i+2].Z);
            PointC.Weight = Weights[i+2];


            PointD.Pos.Input(Pos[i+3].X, Pos[i+3].Y, Pos[i+3].Z);
            PointD.Weight = Weights[i+3];

            TempQuad.PosA = PointA;
            TempQuad.PosB = PointB;
            TempQuad.PosC = PointC;
            TempQuad.PosD = PointD;
            TempQuad.PlaneNorm.X = Normal[i].X;
            TempQuad.PlaneNorm.Y = Normal[i].Y;
            TempQuad.PlaneNorm.Z = Normal[i].Z;
            Output.push_back(TempQuad);

        }

        return Output;
    };

    inline void GeneratePhysicsInfoFromModle(std::vector<FlatFoxPhysics::PhysicsPos> *Pos, std::vector<FlatFoxPhysics::PhysicsPos> *Normal, std::vector<FlatFoxObject::Vertex> *ModleVertexs ){
        PhysicsPos PosTemp, NormalTemp;
        Pos->clear();
        Normal->clear();
        std::vector<FlatFoxObject::Vertex>::iterator I = ModleVertexs->begin();

        for(;I != ModleVertexs->end(); I++){
            PosTemp.VertexPosInput(I->Pos);
            NormalTemp.VertexPosInput(I->NormalPos);

            Pos->push_back(PosTemp);
            Normal->push_back(NormalTemp);
        }
        
    };

    class SimplePhysicsObject{
        protected:
        // This Will mainly be used for the StepPhysicsEnviroment()
        // This can be anytihng but im calling it UUID cause idk its better than ID
        std::string UUID;
        // so skelington 1 can be the same but diffrent form skelington 2
        std::string BaseID;
        public:

        bool IsPlayer;

        unsigned int TYPE{};

        //Used for Internaly updating the object (so an object can technicaly update at faster intervas than the engine if needed)
        float DeltaTime;

        FlatFoxObject::SimpleObject *RenderObject;
        // To make removing objects from vector easier maybe...
        unsigned long EngineVectorPos;

        FlatFoxPhysics::PhysicsPos Position;
        FlatFoxPhysics::PhysicsPos FuturePosition;
        FlatFoxPhysics::PhysicsPos ExtraPositionStorage[4];

        std::vector<FlatFoxPhysics::ForceDirection> AccumulatedAppliedForces;
        FlatFoxPhysics::ForceDirection FinalAppliedForce;
        FlatFoxPhysics::ForceDirection ForceStorageSlot;

        const std::vector<unsigned int> *TypeReactionList;
        std::vector<unsigned int> CustomReactionList;

        std::vector<PhysicsPos> RenderObjectPointPos;
        std::vector<PhysicsPos> RenderObjectPointNormal;
        std::vector<float> RenderObjectPointWight;

        

        std::vector<FlatFoxPhysics::QuadPhysicsBody> QuadPhysicsBodyVector;
        std::vector<FlatFoxPhysics::PhysicsPoint> PhysicsPointVector;
        std::vector<FlatFoxPhysics::SphearPhysicsBody> SphearPhysicsBodyVector;
        

        // I might keep this just to force every Object to have a basic sphear colidor at first
        unsigned int RangeFromCenter{};

        float MinXPos, MinYPos, MaxXPos, MaxYPos;


        // This is just to normalize some expectec/ required inputs for and Physics Objects
        //SimplePhysicsObject(std::string UniqueID = "NULL", std::string TypeID = "NULL", const std::vector<unsigned int> *PhysicsTypeReactionList = nullptr, std::vector<PhysicsPos> *RenderObjPointsPos = nullptr, std::vector<PhysicsPos> *RenderObjPointsNormal = nullptr, std::vector<float> *RenderObjPointsWeight = nullptr, bool Player = false)
        SimplePhysicsObject(std::string UniqueID = "NULL", std::string TypeID = "NULL",  FlatFoxObject::SimpleObject *RenderModle = nullptr, bool Player = false, const std::vector<unsigned int> *PhysicsTypeReactionList = nullptr){
            UUID = std::move(UniqueID);
            BaseID = std::move(TypeID);
            IsPlayer = Player;

            RenderObject = RenderModle;


            this->TypeReactionList = PhysicsTypeReactionList;
            //This will be added to the object by the physics engine later
            //RenderObjectPointPos = RenderObjPointsPos;
            //RenderObjectPointNormal = RenderObjPointsNormal;
            //RenderObjectPointWight = RenderObjPointsWeight;
        }
        ~SimplePhysicsObject(){};
        // Some feneric functons to minipulate the focibly included data
        // This is to work specificaly with the SimpleObject's (render) current output
        inline void SetStarterPosition(std::array<float,3> Pos){
            this->Position.Input(Pos[0],Pos[1],Pos[2]);
            this->FuturePosition = this->Position;
        }

        inline void Move(FlatFoxPhysics::ForceDirection OverrideForce = true){
            if(OverrideForce == true){
                this->FuturePosition = FlatFoxPhysics::MovePhysicsObject(this->Position, FlatFoxPhysics::NormalizeVectorOfForceDirection(this->AccumulatedAppliedForces), this->DeltaTime);
            } else {
                this->FuturePosition = FlatFoxPhysics::MovePhysicsObject(this->Position, OverrideForce, this->DeltaTime, OverrideForce.Speed);
            }

            this->AccumulatedAppliedForces.clear();

        }

        inline void ApplyMovedPosition(){
            this->Position = this->FuturePosition;
        }

        inline void AddQuadPhysicsBody(FlatFoxPhysics::PhysicsPoint A, FlatFoxPhysics::PhysicsPoint B , FlatFoxPhysics::PhysicsPoint C, FlatFoxPhysics::PhysicsPoint D, FlatFoxPhysics::ForceDirection PlaneNormal){
            FlatFoxPhysics::QuadPhysicsBody Temp;
            Temp.PosA = A;
            Temp.PosB = B;
            Temp.PosC = C;
            Temp.PosD = D;
            Temp.PlaneNorm = PlaneNormal;

            this->QuadPhysicsBodyVector.push_back(Temp);
        }
        inline void RemoveQuadPhysicsBody(unsigned int ID){
            this->QuadPhysicsBodyVector.erase(this->QuadPhysicsBodyVector.begin()+ID);
        }

        inline void AddPhysicsPoint(FlatFoxPhysics::PhysicsPos Pos, float Weight) {
            FlatFoxPhysics::PhysicsPoint Temp;
            Temp.Pos = Pos;
            Temp.Weight = Weight;

            this->PhysicsPointVector.push_back(Temp);
        }
        inline void RemovePhysicsPoint(unsigned int ID){
            this->PhysicsPointVector.erase(this->PhysicsPointVector.begin()+ID);
        }

        inline void AddSphearPhysicsBody(FlatFoxPhysics::PhysicsPoint A, float Radius){
            FlatFoxPhysics::SphearPhysicsBody Temp;
            Temp.PosA = A;
            Temp.Radius = Radius;

            this->SphearPhysicsBodyVector.push_back(Temp);
        }
        inline void RemoveSphearPhysicsBody(unsigned int ID){
            this->SphearPhysicsBodyVector.erase(this->SphearPhysicsBodyVector.begin()+ID);
        }

        inline void AddAppliedForce(const FlatFoxPhysics::ForceDirection &Force){
            this->AccumulatedAppliedForces.push_back(Force);
        }

        inline void AddCollisionType(unsigned int type){
            this->CustomReactionList.push_back(type);
        }
        inline void RemoveCollisionType(unsigned int type){
            this->CustomReactionList.erase(this->CustomReactionList.begin() + type);
        }
        inline const std::vector<unsigned int> *ExportCollisionTypes(){
            return &this->CustomReactionList;
        }

        inline void SetParentCollisionList(const std::vector<unsigned int> *TypeList){
            this->TypeReactionList = TypeList;
        }

        inline unsigned int CollisionTypeComparison(unsigned int type){
            for(unsigned int i : *this->TypeReactionList){
                if(type == i){
                    return type;
                }
            }
            for(unsigned int i: this->CustomReactionList){
                if(type == i){
                    return type;
                }
            }
            return -1;
        }

        virtual void OnUpdate() {};

        inline void Update(){
            this->ForceStorageSlot = NormalizeVectorOfForceDirection(this->AccumulatedAppliedForces);


        };
        // Generic colision type resoluton function and operation
        //operator unsigned int(){return TYPE;}
        // This is to give a general idea of a physics update function

        //
    };


// Physics Engine Class

    class PhysicsEngine {
    protected:
        float m_DeltaTime;

        // This is just so the engien know whot to pass user input to
        std::string PlayerID;

        ForceDirection m_Gravity;

        unsigned int m_ThreadLimit;
        float m_ObjectPoolSize; // 0 = auto
        // used for incromenting and keping track of the objects assigned to threads
        unsigned long m_Min = 0, m_Max = 0;

        std::vector<unsigned long> m_ObjThreadTrack;
        std::vector<bool> m_ThreadRunTrack;
        std::vector<bool> m_ThreadKillTrack;



    public:
        inline void SetUpdateTime(float TimePassed) { m_DeltaTime = TimePassed; }

        // The idea is I can ask the rendre object what physics objects are apart of it
        // The Render object in the physics engine are justa referance to the render engine so This should not manage their memory
        std::vector<SimplePhysicsObject*> *PhysicsObjects;

        inline float GetUpdateTime() { return m_DeltaTime; }

        inline unsigned int AllocatedThreads(){return m_ThreadLimit;}

        inline ForceDirection GetGravity() { return m_Gravity; }

        inline void SetPhysicsPlayerID(std::string ID){ PlayerID = std::move(ID);}


        // This is basically reading and writing the position data of the render object but converted to/from physics data
        PhysicsPos GetRenderObjectPhysicsPos(FlatFoxObject::SimpleObject *Object);
        inline void SetRenderObjectPhysicsPos(FlatFoxObject::SimpleObject *Object, PhysicsPos Pos) {Object->SetPosition(Pos.X, Pos.Y, Pos.Z);}

        //inline void MakeRenderdPhysicsObject(SimpleObject* RenderObject) {};

        // Returns froce to move ObjectA to ObjectB
        ForceDirection MakeForceDirection(PhysicsPos ObjectA, PhysicsPos ObjectB);

        void QuadsToLinesVoid(std::vector<QuadPhysicsBody> Object, std::vector<PhysicsLine> *Output);

        std::vector<PhysicsLine> QuadsToLines(std::vector<QuadPhysicsBody> Object);

        void FullQuadLineColisionVoid(std::vector<PhysicsLine> ObjectALines, PhysicsPos ObjectAPos, std::vector<PhysicsLine> ObjectBLines, PhysicsPos ObjectBPos, float Offset, ColisionInfo *Output);

        ColisionInfo FullQuadLineColision(std::vector<PhysicsLine> ObjectALines, PhysicsPos ObjectAPos, std::vector<PhysicsLine> ObjectBLines, PhysicsPos ObjectBPos, float Offset);

        void QuadBodyColision(std::vector<QuadPhysicsBody> ObjectA, PhysicsPos ObjectAPos, std::vector<QuadPhysicsBody> ObjectB, PhysicsPos ObjectBPos, ColisionInfo *Output, float OffSet = 0.0f);
        // Helper functions to make QuadBodyColisoin faster if possible, lol

        // Used for making a vector of PhysicsPoints translated to world space;
        void QuadPosToPoints(std::vector<QuadPhysicsBody> Object, PhysicsPos ObjectPos, std::vector<PhysicsPoint> *Output);

        // With Normal
        void QuadPosToPointsNormal(std::vector<QuadPhysicsBody> Object, PhysicsPos ObjectPos, std::vector<PhysicsLine> *Output);

        // Normalizes returns normal for ObjectBs points pointing twords ObjectAs points
        void PointsToPointsNormal(std::vector<PhysicsPos> ObjectA, std::vector<PhysicsPos> ObjectB, std::vector<PhysicsPos> *Output);

        // Effectivly the samthing as the Normal one but for indevidual Positions
        void PointToPoint(PhysicsPos *ObjectA, PhysicsPos *ObjectB, PhysicsPos *Output);

        void LinesToLines(std::vector<PhysicsLine> *ObjectA, std::vector<PhysicsLine> *ObjectB, std::vector<PhysicsLine> *Output);

        //Currently breaks on the Z axis (it only works in 2d some how...)
        ColisionInfo SATColision(std::vector<QuadPhysicsBody> ObjectA, PhysicsPos ObjectAPos, std::vector<QuadPhysicsBody> ObjectB, PhysicsPos ObjectBPos);

        // The returned minmax is based on world cordinates
        std::vector<PlaneMinMax> MinMaxFromQuads(std::vector<QuadPhysicsBody> Object, PhysicsPos ObjectPos);

        // This Function only does the comparison 1 way : PointsA to BoxB
        ColisionInfo PointsToAABBColision(std::vector<QuadPhysicsBody> ObjectA, PhysicsPos ObjectAPos, std::vector<PlaneMinMax> ObjectB);

        void AABBColision(std::vector<QuadPhysicsBody> &ObjectA, PhysicsPos &ObjectAPos, std::vector<QuadPhysicsBody> &ObjectB, PhysicsPos &ObjectBPos, ColisionInfo *Output);

        void SphearColison(PhysicsPos ObjectAPos, float ObjectASize, PhysicsPos ObjectBPos, float ObjectBSize, ColisionInfo *Output);

        // IDK why i didn't make this earlier... 
        float PointToPointDistance(PhysicsPos PointeA, PhysicsPos PointB);



        float DotPointToForce(PhysicsPos Point, ForceDirection Projection, PhysicsPos ProjectionPos);

        PhysicsPos Center2Point(PhysicsPos PointA, PhysicsPos PointB);

        PhysicsPos Center4Point(PhysicsPos *PointA, PhysicsPos *PointB, PhysicsPos *PointC, PhysicsPos *PointD);

        ColisionInfo SphearToPlane(SphearPhysicsBody ObjectA, QuadPhysicsBody ObjectB, PhysicsPos ObjectBPos);



        void NormalizeForceDirection(ForceDirection ForceA, ForceDirection ForceB, ForceDirection *Output);
       // Main decliration function for the physics engine
        PhysicsEngine(float GravityForce = 0.0f, float GravityX =0.0f, float GravityY = 0.0f, float GravityZ = 0.0f, unsigned int ThreadPool = 0);

        ~PhysicsEngine();

        void InternalUpdate(SimplePhysicsObject *Object ,std::vector<SimplePhysicsObject *> *Objects, unsigned long *Start, unsigned long *Stop);

        void ThreadSkelington(std::vector<SimplePhysicsObject *> *Objects, unsigned long *Start, unsigned long *End, bool *Run, bool *Kill);

        // Just so decliratinos can be copied an updated later
        inline bool SubThreadSwitchStarter(bool StartState){return StartState;};

        // Add and Remove PhysicsObjects
        void AddSimplePhysicsObject(SimplePhysicsObject *NewObject);
        void RemoveSimplePhysicsObject(unsigned long ObjectPOSID);

        inline void SetPhysicsObjectVectorPointer(std::vector<SimplePhysicsObject*> *VectorPtr){
            this->PhysicsObjects = VectorPtr;
        }

        void Update(FlatFoxPhysics::ForceDirection UserInput);


        void SimpleThreadTest();


    };
}