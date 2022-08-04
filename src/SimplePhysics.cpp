#include "SimplePhysics.hpp"
using namespace FlatFoxPhysics;


PhysicsEngine::PhysicsEngine(float GravityForce, float GravityX, float GravityY, float GravityZ, unsigned int ThreadPool){
    if(ThreadPool == 0){
        m_ThreadLimit = std::thread::hardware_concurrency() * 2;
    } else {
        m_ThreadLimit = ThreadPool;
    }

    for(unsigned int i = 0; i < m_ThreadLimit; i++){
        m_ThreadRunTrack.push_back(this->SubThreadSwitchStarter(false));
        m_ThreadKillTrack.push_back(this->SubThreadSwitchStarter(false));
        //ThreadSkelington(&this->Objects, &this->m_Min, &this->m_Max, this->m_ThreadRunTrack[m_ThreadRunTrack.size()], this->m_ThreadKillTrack[m_ThreadKillTrack.size()]);
    }

    float Length = std::sqrt(GravityX * GravityX + GravityY * GravityY + GravityZ * GravityZ);

    if(Length != 0.){
        GravityX /= Length;
        GravityY /= Length;
        GravityZ /= Length;
    }

    m_Gravity.X = GravityX;
    m_Gravity.Y = GravityY;
    m_Gravity.Z = GravityZ;

    m_Gravity.Speed = GravityForce;

    PhysicsObjects.clear();
    PhysicsObjects.resize(1);
}

PhysicsEngine::~PhysicsEngine(){
    for(unsigned long i = 0; i < m_ThreadKillTrack.size(); i++){
        m_ThreadKillTrack[i] = true;
    }

}

/*void PhysicsEngine::MakeRenderdPhysicsObject(SimpleObject *RenderObject) {
    // junk just to check for compile
    return;
}*/

PhysicsPos PhysicsEngine::GetRenderObjectPhysicsPos(FlatFoxObject::SimpleObject *Object){
    PhysicsPos Temp;
    Temp.X = Object->GetPosAlt().at(0);
    Temp.Y = Object->GetPosAlt().at(1);
    Temp.Z = Object->GetPosAlt().at(2);
    return Temp;
}

ForceDirection PhysicsEngine::MakeForceDirection(PhysicsPos ObjectA, PhysicsPos ObjectB){
    ForceDirection Output;
    float Length;
    Output.X = ObjectB.X - ObjectA.X;
    Output.Y = ObjectB.Y - ObjectA.Y;
    Output.Z = ObjectB.Z - ObjectA.Z;

    Length = std::sqrt(Output.X * Output.X + Output.Y * Output.Y + Output.Z * Output.Z);

    if(Length != 0.){
        Output.X /= Length;
        Output.Y /= Length;
        Output.Z /= Length;
    }

    return Output;
}

void PhysicsEngine::QuadsToLinesVoid(std::vector<QuadPhysicsBody> Object ,std::vector<PhysicsLine> *Output){
    PhysicsLine Temp;

    for(long unsigned int i = 0; i < Object.size(); i++){
        Temp.PosA = Object[i].PosA;
        Temp.PosB = Object[i].PosB;
        Temp.Diff = std::sqrt(((Temp.PosB.Pos.X - Temp.PosA.Pos.X)*(Temp.PosB.Pos.X - Temp.PosA.Pos.X)) + ((Temp.PosB.Pos.Y - Temp.PosA.Pos.Y)*(Temp.PosB.Pos.Y - Temp.PosA.Pos.Y)) + ((Temp.PosB.Pos.Z - Temp.PosA.Pos.Z)*(Temp.PosB.Pos.Z - Temp.PosA.Pos.Z)));
        Temp.Normal = Object[i].PlaneNorm;

        Output->push_back(Temp);

        Temp.PosA = Object[i].PosB;
        Temp.PosB = Object[i].PosC;
        Temp.Diff = std::sqrt(((Temp.PosB.Pos.X - Temp.PosA.Pos.X)*(Temp.PosB.Pos.X - Temp.PosA.Pos.X)) + ((Temp.PosB.Pos.Y - Temp.PosA.Pos.Y)*(Temp.PosB.Pos.Y - Temp.PosA.Pos.Y)) + ((Temp.PosB.Pos.Z - Temp.PosA.Pos.Z)*(Temp.PosB.Pos.Z - Temp.PosA.Pos.Z)));
        Temp.Normal = Object[i].PlaneNorm;

        Output->push_back(Temp);

        Temp.PosA = Object[i].PosC;
        Temp.PosB = Object[i].PosD;
        Temp.Diff = std::sqrt(((Temp.PosB.Pos.X - Temp.PosA.Pos.X)*(Temp.PosB.Pos.X - Temp.PosA.Pos.X)) + ((Temp.PosB.Pos.Y - Temp.PosA.Pos.Y)*(Temp.PosB.Pos.Y - Temp.PosA.Pos.Y)) + ((Temp.PosB.Pos.Z - Temp.PosA.Pos.Z)*(Temp.PosB.Pos.Z - Temp.PosA.Pos.Z)));
        Temp.Normal = Object[i].PlaneNorm;

        Output->push_back(Temp);

        Temp.PosA = Object[i].PosD;
        Temp.PosB = Object[i].PosA;
        Temp.Diff = std::sqrt( ((Temp.PosB.Pos.X - Temp.PosA.Pos.X)*(Temp.PosB.Pos.X - Temp.PosA.Pos.X)) + ((Temp.PosB.Pos.Y - Temp.PosA.Pos.Y)*(Temp.PosB.Pos.Y - Temp.PosA.Pos.Y)) + ((Temp.PosB.Pos.Z - Temp.PosA.Pos.Z)*(Temp.PosB.Pos.Z - Temp.PosA.Pos.Z)));
        Temp.Normal = Object[i].PlaneNorm;

        Output->push_back(Temp);

    }

    //return Output;
}

// Each Quad has 6 PhysicsLines generated from the 4 points
std::vector<PhysicsLine> PhysicsEngine::QuadsToLines(std::vector<QuadPhysicsBody> Object){
    std::vector<PhysicsLine> Output;
    PhysicsLine Temp;

    for(long unsigned int i = 0; i < Object.size(); i++){
        Temp.PosA = Object[i].PosA;
        Temp.PosB = Object[i].PosB;
        Temp.Diff = std::sqrt( ((Temp.PosB.Pos.X - Temp.PosA.Pos.X)*(Temp.PosB.Pos.X - Temp.PosA.Pos.X)) + ((Temp.PosB.Pos.Y - Temp.PosA.Pos.Y)*(Temp.PosB.Pos.Y - Temp.PosA.Pos.Y)) + ((Temp.PosB.Pos.Z - Temp.PosA.Pos.Z)*(Temp.PosB.Pos.Z - Temp.PosA.Pos.Z)));
        Temp.Normal = Object[i].PlaneNorm;

        Output.push_back(Temp);

        Temp.PosA = Object[i].PosB;
        Temp.PosB = Object[i].PosC;
        Temp.Diff = std::sqrt(((Temp.PosB.Pos.X - Temp.PosA.Pos.X)*(Temp.PosB.Pos.X - Temp.PosA.Pos.X)) + ((Temp.PosB.Pos.Y - Temp.PosA.Pos.Y)*(Temp.PosB.Pos.Y - Temp.PosA.Pos.Y)) + ((Temp.PosB.Pos.Z - Temp.PosA.Pos.Z)*(Temp.PosB.Pos.Z - Temp.PosA.Pos.Z)));
        Temp.Normal = Object[i].PlaneNorm;

        Output.push_back(Temp);

        Temp.PosA = Object[i].PosC;
        Temp.PosB = Object[i].PosD;
        Temp.Diff = std::sqrt(((Temp.PosB.Pos.X - Temp.PosA.Pos.X)*(Temp.PosB.Pos.X - Temp.PosA.Pos.X)) + ((Temp.PosB.Pos.Y - Temp.PosA.Pos.Y)*(Temp.PosB.Pos.Y - Temp.PosA.Pos.Y)) + ((Temp.PosB.Pos.Z - Temp.PosA.Pos.Z)*(Temp.PosB.Pos.Z - Temp.PosA.Pos.Z)));
        Temp.Normal = Object[i].PlaneNorm;

        Output.push_back(Temp);

        Temp.PosA = Object[i].PosD;
        Temp.PosB = Object[i].PosA;
        Temp.Diff = std::sqrt(((Temp.PosB.Pos.X - Temp.PosA.Pos.X)*(Temp.PosB.Pos.X - Temp.PosA.Pos.X)) + ((Temp.PosB.Pos.Y - Temp.PosA.Pos.Y)*(Temp.PosB.Pos.Y - Temp.PosA.Pos.Y)) + ((Temp.PosB.Pos.Z - Temp.PosA.Pos.Z)*(Temp.PosB.Pos.Z - Temp.PosA.Pos.Z)));
        Temp.Normal = Object[i].PlaneNorm;

        Output.push_back(Temp);
        // IDK if i need these
        /*
        Temp.PosA = Object[i].PosA;
        Temp.PosB = Object[i].PosC;
        Temp.Diff = std::sqrt( ((Temp.PosB.X - Temp.PosA.X)*(Temp.PosB.X - Temp.PosA.X)) + ((Temp.PosB.Y - Temp.PosA.Y)*(Temp.PosB.Y - Temp.PosA.Y)) + ((Temp.PosB.Z - Temp.PosA.Z)*(Temp.PosB.Z - Temp.PosA.Z)) );
        Temp.Normal = Object[i].PlaneNorm;

        Output.push_back(Temp);

        Temp.PosA = Object[i].PosB;
        Temp.PosB = Object[i].PosD;
        Temp.Diff = std::sqrt( ((Temp.PosB.X - Temp.PosA.X)*(Temp.PosB.X - Temp.PosA.X)) + ((Temp.PosB.Y - Temp.PosA.Y)*(Temp.PosB.Y - Temp.PosA.Y)) + ((Temp.PosB.Z - Temp.PosA.Z)*(Temp.PosB.Z - Temp.PosA.Z)) );
        Temp.Normal = Object[i].PlaneNorm;

        Output.push_back(Temp);
        */


    }

    return Output;
}

void PhysicsEngine::FullQuadLineColisionVoid(std::vector<PhysicsLine> ObjectALines, PhysicsPos ObjectAPos, std::vector<PhysicsLine> ObjectBLines, PhysicsPos ObjectBPos, float Offset, ColisionInfo *Output){
    // Assume no colision
    Output->IsColision = false;
    float A,B,C,D;

    PhysicsPos Line1A, Line1B, Line2A, Line2B;

    for(long unsigned int i = 0; i < ObjectALines.size(); i++){
        for(long unsigned int j = 0; j < ObjectBLines.size(); j++){
            Line1A.X = ObjectALines[i].PosA.Pos.X + ObjectAPos.X;
            Line1A.Y = ObjectALines[i].PosA.Pos.Y + ObjectAPos.Y;
            Line1A.Z = ObjectALines[i].PosA.Pos.Z + ObjectAPos.Z;

            Line1B.X = ObjectALines[i].PosB.Pos.X + ObjectAPos.X;
            Line1B.Y = ObjectALines[i].PosB.Pos.Y + ObjectAPos.Y;
            Line1B.Z = ObjectALines[i].PosB.Pos.Z + ObjectAPos.Z;

            Line2A.X = ObjectBLines[j].PosA.Pos.X + ObjectBPos.X;
            Line2A.Y = ObjectBLines[j].PosA.Pos.Y + ObjectBPos.Y;
            Line2A.Z = ObjectBLines[j].PosA.Pos.Z + ObjectBPos.Z;

            Line2B.X = ObjectBLines[j].PosB.Pos.X + ObjectBPos.X;
            Line2B.Y = ObjectBLines[j].PosB.Pos.Y + ObjectBPos.Y;
            Line2B.Z = ObjectBLines[j].PosB.Pos.Z + ObjectBPos.Z;

            A =  std::sqrt( (Line2A.X - Line1A.X)*(Line2A.X-Line1A.X) + (Line2A.Y - Line1A.Y)*(Line2A.Y - Line1A.Y) + (Line2A.Z - Line1A.Z)*(Line2A.Z - Line1A.Z));
            B =  std::sqrt( (Line2B.X - Line1A.X)*(Line2B.X-Line1A.X) + (Line2B.Y - Line1A.Y)*(Line2B.Y - Line1A.Y) + (Line2B.Z - Line1A.Z)*(Line2B.Z - Line1A.Z));
            C =  std::sqrt( (Line2A.X - Line1B.X)*(Line2A.X-Line1B.X) + (Line2A.Y - Line1B.Y)*(Line2A.Y - Line1B.Y) + (Line2A.Z - Line1B.Z)*(Line2A.Z - Line1B.Z));
            D =  std::sqrt( (Line2B.X - Line1B.X)*(Line2B.X-Line1B.X) + (Line2B.Y - Line1B.Y)*(Line2B.Y - Line1B.Y) + (Line2B.Z - Line1B.Z)*(Line2B.Z - Line1B.Z));

            if((A+B) <= (ObjectBLines[j].Diff * Offset) && (C+D) <= (ObjectBLines[j].Diff * Offset)){
                Output->IsColision = true;
                Output->MovmentDirectionA = ObjectALines[i].Normal;
                Output->MovmentDirectionB = ObjectBLines[j].Normal;
                Output->Force = 2.0f;
                break;
            }

        }
        if(Output->IsColision){
            break;
        }

    }

}

ColisionInfo PhysicsEngine::FullQuadLineColision(std::vector<PhysicsLine> ObjectALines, PhysicsPos ObjectAPos, std::vector<PhysicsLine> ObjectBLines, PhysicsPos ObjectBPos, float Offset){
    ColisionInfo Output;
    Output.IsColision = false;
    float A,B,C,D;

    PhysicsPos Line1A, Line1B, Line2A, Line2B;

  

    for(long unsigned int i = 0; i < ObjectALines.size(); i++){
        for(long unsigned int j = 0; j < ObjectBLines.size(); j++){
            Line1A.X = ObjectALines[i].PosA.Pos.X + ObjectAPos.X;
            Line1A.Y = ObjectALines[i].PosA.Pos.Y + ObjectAPos.Y;
            Line1A.Z = ObjectALines[i].PosA.Pos.Z + ObjectAPos.Z;

            Line1B.X = ObjectALines[i].PosB.Pos.X + ObjectAPos.X;
            Line1B.Y = ObjectALines[i].PosB.Pos.Y + ObjectAPos.Y;
            Line1B.Z = ObjectALines[i].PosB.Pos.Z + ObjectAPos.Z;

            Line2A.X = ObjectBLines[j].PosA.Pos.X + ObjectBPos.X;
            Line2A.Y = ObjectBLines[j].PosA.Pos.Y + ObjectBPos.Y;
            Line2A.Z = ObjectBLines[j].PosA.Pos.Z + ObjectBPos.Z;

            Line2B.X = ObjectBLines[j].PosB.Pos.X + ObjectBPos.X;
            Line2B.Y = ObjectBLines[j].PosB.Pos.Y + ObjectBPos.Y;
            Line2B.Z = ObjectBLines[j].PosB.Pos.Z + ObjectBPos.Z;

            A =  std::sqrt( (Line2A.X - Line1A.X)*(Line2A.X-Line1A.X) + (Line2A.Y - Line1A.Y)*(Line2A.Y - Line1A.Y) + (Line2A.Z - Line1A.Z)*(Line2A.Z - Line1A.Z));
            B =  std::sqrt( (Line2B.X - Line1A.X)*(Line2B.X-Line1A.X) + (Line2B.Y - Line1A.Y)*(Line2B.Y - Line1A.Y) + (Line2B.Z - Line1A.Z)*(Line2B.Z - Line1A.Z));
            C =  std::sqrt( (Line2A.X - Line1B.X)*(Line2A.X-Line1B.X) + (Line2A.Y - Line1B.Y)*(Line2A.Y - Line1B.Y) + (Line2A.Z - Line1B.Z)*(Line2A.Z - Line1B.Z));
            D =  std::sqrt( (Line2B.X - Line1B.X)*(Line2B.X-Line1B.X) + (Line2B.Y - Line1B.Y)*(Line2B.Y - Line1B.Y) + (Line2B.Z - Line1B.Z)*(Line2B.Z - Line1B.Z));
            //std::cout << "Just some Numbers" << std::endl;
            //std::cout << A+B << " | " << ObjectBLines[i].Diff * Offset << std::endl;
            //std::cout << C+D << " | " << ObjectBLines[i].Diff << std::endl;
            if((A+B) <= (ObjectBLines[j].Diff * Offset) && (C+D) <= (ObjectBLines[j].Diff * Offset)){
                //std::cout << A << " | " << B << " | " << C << " | " << D  << std::endl;
                /*
                std::cout << "Colision diffs" << std::endl;
                std::cout << i << "/" << ObjectALines.size() << " | " << j <<"/"<< ObjectBLines.size() << std::endl;
                std::cout << ObjectALines[i].PosA.X << " | " <<  ObjectALines[i].PosA.Y << " | " << ObjectALines[i].PosA.Z << std::endl;
                std::cout << "2 other points" << std::endl;
                std::cout << ObjectBLines[i].PosA.X << " | " <<  ObjectBLines[i].PosA.Y << " | " << ObjectBLines[i].PosA.Z << std::endl;
                std::cout << ObjectBLines[i].PosB.X << " | " <<  ObjectBLines[i].PosB.Y << " | " << ObjectBLines[i].PosB.Z << std::endl;

                
                std::cout << "Added off set" << std::endl;
                std::cout << ObjectAPos.X << " | " << ObjectAPos.Y << " | " << ObjectAPos.Z << std::endl;
                std::cout << ObjectBPos.X << " | " << ObjectBPos.Y << " | " << ObjectBPos.Z << std::endl;  
                std::cout << A << " | " << B << " | " << C << " | " << D  << std::endl;
                std::cout << A+B << " | " << ObjectBLines[j].Diff * Offset << std::endl;
                std::cout << C+D << " | " << ObjectBLines[j].Diff << std::endl; 
                */
                Output.IsColision = true;
                Output.MovmentDirectionA = ObjectALines[i].Normal;
                Output.MovmentDirectionB = ObjectBLines[j].Normal;
                Output.Force = 2.0f;
                break;
            }

        }
        if(Output.IsColision){
            break;
        }

    }

    if(!Output.IsColision){
        for(long unsigned int i = 0; i < ObjectBLines.size(); i++){
            for(long unsigned int j = 0; j < ObjectALines.size(); j++){
                Line1A.X = ObjectALines[j].PosA.Pos.X + ObjectAPos.X;
                Line1A.Y = ObjectALines[j].PosA.Pos.Y + ObjectAPos.Y;
                Line1A.Z = ObjectALines[j].PosA.Pos.Z + ObjectAPos.Z;

                Line1B.X = ObjectALines[j].PosB.Pos.X + ObjectAPos.X;
                Line1B.Y = ObjectALines[j].PosB.Pos.Y + ObjectAPos.Y;
                Line1B.Z = ObjectALines[j].PosB.Pos.Z + ObjectAPos.Z;

                Line2A.X = ObjectBLines[i].PosA.Pos.X + ObjectBPos.X;
                Line2A.Y = ObjectBLines[i].PosA.Pos.Y + ObjectBPos.Y;
                Line2A.Z = ObjectBLines[i].PosA.Pos.Z + ObjectBPos.Z;

                Line2B.X = ObjectBLines[i].PosB.Pos.X + ObjectBPos.X;
                Line2B.Y = ObjectBLines[i].PosB.Pos.Y + ObjectBPos.Y;
                Line2B.Z = ObjectBLines[i].PosB.Pos.Z + ObjectBPos.Z;

                A =  std::sqrt( (Line2A.X - Line1A.X)*(Line2A.X-Line1A.X) + (Line2A.Y - Line1A.Y)*(Line2A.Y - Line1A.Y) + (Line2A.Z - Line1A.Z)*(Line2A.Z - Line1A.Z));
                B =  std::sqrt( (Line2B.X - Line1A.X)*(Line2B.X-Line1A.X) + (Line2B.Y - Line1A.Y)*(Line2B.Y - Line1A.Y) + (Line2B.Z - Line1A.Z)*(Line2B.Z - Line1A.Z));
                C =  std::sqrt( (Line2A.X - Line1B.X)*(Line2A.X-Line1B.X) + (Line2A.Y - Line1B.Y)*(Line2A.Y - Line1B.Y) + (Line2A.Z - Line1B.Z)*(Line2A.Z - Line1B.Z));
                D =  std::sqrt( (Line2B.X - Line1B.X)*(Line2B.X-Line1B.X) + (Line2B.Y - Line1B.Y)*(Line2B.Y - Line1B.Y) + (Line2B.Z - Line1B.Z)*(Line2B.Z - Line1B.Z));

                if((A+C) <= (ObjectALines[j].Diff * Offset) && (B+D) <= (ObjectALines[j].Diff * Offset)){
                    Output.IsColision = true;
                    Output.MovmentDirectionA = ObjectALines[j].Normal;
                    Output.MovmentDirectionB = ObjectBLines[i].Normal;
                    Output.Force = 2.0f;
                    break;
                }

            }
            if(Output.IsColision){
                break;
            }

        }
    }
    


    return Output;
}

void PhysicsEngine::QuadBodyColision(std::vector<QuadPhysicsBody> ObjectA, PhysicsPos ObjectAPos, std::vector<QuadPhysicsBody> ObjectB, PhysicsPos ObjectBPos, ColisionInfo *Output, float OffSet){
    Output->IsColision = false;
    
    // Each point after being translated to its world Pos
    std::vector<PhysicsLine> TranslatedA, TranslatedB;
    
    std::vector<PhysicsLine> Compare, AltCompare;
    /*
    float XMin, YMin, ZMin;t
    float XMax, YMax, ZMax;



    ZMin = INFINITY;

    XMax = -INFINITY;
    YMax = -INFINITY;
    ZMax = -INFINITY;

    */


    PhysicsEngine::QuadPosToPointsNormal(ObjectA, ObjectAPos, &TranslatedA); 
    PhysicsEngine::QuadPosToPointsNormal(ObjectB, ObjectBPos, &TranslatedB);

    //TranslationThreadA.join();
    //TranslationThreadB.join();


    PhysicsEngine::LinesToLines(&TranslatedA, &TranslatedB, &Compare);
    //PhysicsEngine::PointsToPointsNormal(TranslatedB, TranslatedA, &AltCompare);

    

    /*
    for(int i = 0; i < Compare.size(); i++){
        if(Compare[i].X < XMin){
            XMin = Compare[i].X;
        }
        if(Compare[i].Y < YMin){
            YMin = Compare[i].Y;
        }
        if(Compare[i].Z < ZMin){
            ZMin = Compare[i].Z;
        }

        if(Compare[i].X > XMax){
            XMax = Compare[i].X;
        }
        if(Compare[i].Y > YMax){
            YMax = Compare[i].Y;
        }
        if(Compare[i].Z > ZMax){
            ZMax = Compare[i].Z;
        }
    }


    if((XMin < 0.0f && XMax > 0.0f) && (YMin < 0.0f && YMax > 0.0f)  && (ZMin < 0.0f && ZMax > 0.0f)){
        Output.IsColision = true;
        // This just some place holder information
        Output.MovmentDirectionA.X =  TranslatedA[0].X;
        Output.MovmentDirectionA.Y =  TranslatedA[0].Y;
        Output.MovmentDirectionA.Z =  TranslatedA[0].Z;

        Output.MovmentDirectionB.X =  TranslatedB[0].X;
        Output.MovmentDirectionB.Y =  TranslatedB[0].Y;
        Output.MovmentDirectionB.Z =  TranslatedB[0].Z;
    }
    */
    //Generate Base line
    PhysicsPoint BasePoint;
    // In this case Positive is true and negative is false;
    bool BaseXPosNeg, BaseYPosNeg, BaseZPosNeg;
    BasePoint.Pos.X = ObjectBPos.X - ObjectAPos.X;
    BasePoint.Pos.Y = ObjectBPos.Y - ObjectAPos.Y;
    BasePoint.Pos.Z = ObjectBPos.Z - ObjectAPos.Z; 

    if(BasePoint.Pos.X < OffSet){
        BaseXPosNeg = false;
    } else {
        BaseXPosNeg = true;
    }

    if(BasePoint.Pos.Y < OffSet){
        BaseYPosNeg = false;
    } else {
        BaseYPosNeg = true;
    }

    if(BasePoint.Pos.Z < OffSet){
        BaseZPosNeg = false;
    } else {
        BaseZPosNeg = true;
    }

    // Same as the Base[]PosNeg, Positive is true and negative is false;
    bool XPosNeg, YPosNeg, ZPosNeg;

    for(long unsigned int i = 0; i < Compare.size(); i++){
        if(Compare[i].PosA.Pos.X < 0.0f){
            XPosNeg = false;
        } else {
            XPosNeg = true;
        }

        if(Compare[i].PosA.Pos.Y < 0.0f){
            YPosNeg = false;
        } else {
            YPosNeg = true;
        }

        if(Compare[i].PosA.Pos.Z < 0.0f){
            ZPosNeg = false;
        } else {
            ZPosNeg = true;
        }

        //TODO : Make Proper peroint colission output;
        if((BaseXPosNeg != XPosNeg) && (BaseYPosNeg != YPosNeg) && (BaseZPosNeg != ZPosNeg)){
            Output->IsColision = true;
        // This just some place holder information
            Output->PastMovmentDirectionA = Output->MovmentDirectionA;
            Output->PastMovmentDirectionB = Output->MovmentDirectionB;
            Output->MovmentDirectionA = MakeForceDirection(Compare[i].PosA.Pos, BasePoint.Pos);
            Output->MovmentDirectionB = MakeForceDirection(BasePoint.Pos, Compare[i].PosA.Pos);
            //Output.MovmentDirectionA.X = Compare[i].PosB.X;
            //Output.MovmentDirectionA.Y = Compare[i].PosB.Y;
            //Output.MovmentDirectionA.Z = Compare[i].PosB.Z;

            //Output.MovmentDirectionB.X = -1.0f*Compare[i].PosB.X;
            //Output.MovmentDirectionB.Y = -1.0f*Compare[i].PosB.Y;
            //Output.MovmentDirectionB.Z = -1.0f*Compare[i].PosB.Z;
            return;
        }

    }
    


    
    

    return;
}

void PhysicsEngine::QuadPosToPoints(std::vector<QuadPhysicsBody> Object, PhysicsPos ObjectPos, std::vector<PhysicsPoint> *Output){
    PhysicsPoint Temp;
    for(long unsigned int i = 0; i < Object.size(); i++){
        Temp.Pos.Input((Object[i].PosA.Pos.X + ObjectPos.X), (Object[i].PosA.Pos.Y + ObjectPos.Y), (Object[i].PosA.Pos.Z + ObjectPos.Z));
        Temp.Weight = Object[i].PosA.Weight;

        Output->push_back(Temp);


        Temp.Pos.Input((Object[i].PosB.Pos.X + ObjectPos.X), (Object[i].PosB.Pos.Y + ObjectPos.Y), (Object[i].PosB.Pos.Z + ObjectPos.Z));
        Temp.Weight = Object[i].PosB.Weight;

        Output->push_back(Temp);


        Temp.Pos.Input((Object[i].PosC.Pos.X + ObjectPos.X), (Object[i].PosC.Pos.Y + ObjectPos.Y), (Object[i].PosC.Pos.Z + ObjectPos.Z));
        Temp.Weight = Object[i].PosC.Weight;

        Output->push_back(Temp);


        Temp.Pos.Input((Object[i].PosD.Pos.X + ObjectPos.X), (Object[i].PosD.Pos.Y + ObjectPos.Y), (Object[i].PosD.Pos.Z + ObjectPos.Z));
        Temp.Weight = Object[i].PosD.Weight;

        Output->push_back(Temp);

    }
}



void PhysicsEngine::QuadPosToPointsNormal(std::vector<QuadPhysicsBody> Object, PhysicsPos ObjectPos, std::vector<PhysicsLine> *Output){
    PhysicsLine Temp;
    //PhysicsPoint TempPoint;
    for(long unsigned int i = 0; i < Object.size(); i++){
        //TempPoint = Center4Point(&Object[i].PosA, &Object[i].PosB, &Object[i].PosC, &Object[i].PosD);

        Temp.PosA.Pos.Input((Object[i].PosA.Pos.X + ObjectPos.X), (Object[i].PosA.Pos.Y + ObjectPos.Y), (Object[i].PosA.Pos.Z + ObjectPos.Z));
        Temp.PosB.Pos.Input(Object[i].PlaneNorm.X, Object[i].PlaneNorm.Y, Object[i].PlaneNorm.Z);
        //Temp.PosB = TempPoint;
        Temp.PosA.Weight = Object[i].PosA.Weight;

        Output->push_back(Temp);


        Temp.PosA.Pos.Input((Object[i].PosB.Pos.X + ObjectPos.X), (Object[i].PosB.Pos.Y + ObjectPos.Y), (Object[i].PosB.Pos.Z + ObjectPos.Z));
        Temp.PosB.Pos.Input(Object[i].PlaneNorm.X, Object[i].PlaneNorm.Y, Object[i].PlaneNorm.Z);
        //Temp.PosB = TempPoint;
        Temp.PosA.Weight = Object[i].PosB.Weight;

        Output->push_back(Temp);


        Temp.PosA.Pos.Input((Object[i].PosC.Pos.X + ObjectPos.X), (Object[i].PosC.Pos.Y + ObjectPos.Y), (Object[i].PosC.Pos.Z + ObjectPos.Z));
        Temp.PosB.Pos.Input(Object[i].PlaneNorm.X, Object[i].PlaneNorm.Y, Object[i].PlaneNorm.Z);
        //Temp.PosB = TempPoint;
        Temp.PosA.Weight = Object[i].PosC.Weight;

        Output->push_back(Temp);


        Temp.PosA.Pos.Input((Object[i].PosD.Pos.X + ObjectPos.X), (Object[i].PosD.Pos.Y + ObjectPos.Y), (Object[i].PosD.Pos.Z + ObjectPos.Z));
        Temp.PosB.Pos.Input(Object[i].PlaneNorm.X, Object[i].PlaneNorm.Y, Object[i].PlaneNorm.Z);
        //Temp.PosB = TempPoint;
        Temp.PosA.Weight = Object[i].PosD.Weight;

        Output->push_back(Temp);

    }
}





// Normalizes ObjectAs points to ObjectBs points
void PhysicsEngine::PointsToPointsNormal(std::vector<PhysicsPos> ObjectA, std::vector<PhysicsPos> ObjectB, std::vector<PhysicsPos> *Output){
    PhysicsPos Temp;
    for(long unsigned int i = 0; i < ObjectA.size(); i++){
        for(long unsigned int j = 0; j < ObjectB.size(); j++){
            Temp.Input((ObjectB[j].X - ObjectA[i].X), (ObjectB[j].Y - ObjectA[i].Y), (ObjectB[j].Z -  ObjectA[i].Z));
            Output->push_back(Temp);
        }
    }
}

void PhysicsEngine::PointToPoint(PhysicsPos *ObjectA, PhysicsPos *ObjectB, PhysicsPos *Output){

        Output->Input((ObjectB->X - ObjectA->X), (ObjectB->Y - ObjectA->Y), (ObjectB->Z - ObjectA->Z));

}

void PhysicsEngine::LinesToLines(std::vector<PhysicsLine> *ObjectA, std::vector<PhysicsLine> *ObjectB, std::vector<PhysicsLine> *Output){
    PhysicsLine Temp;

    for(long unsigned int i = 0; i < ObjectA->size(); i++){
        for(long unsigned int j = 0; j < ObjectB->size(); j++){
            PhysicsEngine::PointToPoint(&ObjectA->at(i).PosA.Pos, &ObjectB->at(j).PosA.Pos, &Temp.PosA.Pos);
            Output->push_back(Temp);
            PhysicsEngine::PointToPoint(&ObjectA->at(i).PosA.Pos, &ObjectB->at(j).PosB.Pos, &Temp.PosB.Pos);
            Output->push_back(Temp);
        }
    }
}

ColisionInfo PhysicsEngine::SATColision(std::vector<QuadPhysicsBody> ObjectA, PhysicsPos ObjectAPos, std::vector<QuadPhysicsBody> ObjectB, PhysicsPos ObjectBPos){
    ColisionInfo Output;
    Output.IsColision = false; 

    std::vector<float> ObjectAShadow, ObjectBShadow;
    PhysicsPos TempAPoint, TempBPoint, CenterPoint;
    float ObjectAMin = INFINITY;
    float ObjectAMax = -INFINITY;
    float ObjectBMin = INFINITY;
    float ObjectBMax = -INFINITY;
    // Take the norm or each polly and crush each point to 1D to compare
    // The origion of the vector will be the origin of the object
    for(long unsigned int i = 0; i < ObjectA.size(); i++){
        //CenterPointA.X = ObjectA[i].PosA.X + ObjectAPos.X;
        //CenterPointA.Y = ObjectA[i].PosA.Y + ObjectAPos.Y;
        //CenterPointA.Z = ObjectA[i].PosA.Z + ObjectAPos.Z;

        //CenterPointB.X = ObjectA[i].PosD.X + ObjectAPos.X;
        //CenterPointB.Y = ObjectA[i].PosD.Y + ObjectAPos.Y;
        //CenterPointB.Z = ObjectA[i].PosD.Z + ObjectAPos.Z;

        //CenterPoint = PhysicsEngine::CenterPoint(CenterPointA, CenterPointB);
        CenterPoint.Input(0.0f,0.0f,0.0f);

        for(long unsigned int j = 0; j < ObjectA.size(); j++){
            for(int n = 0; n < 4; n++){
                switch(n){
                    case 0:
                        TempAPoint.Input((ObjectA[j].PosA.Pos.X + ObjectAPos.X), (ObjectA[j].PosA.Pos.Y + ObjectAPos.Y), (ObjectA[j].PosA.Pos.Z + ObjectAPos.Z));
                        break;
                    case 1:
                        TempAPoint.Input((ObjectA[j].PosB.Pos.X + ObjectAPos.X), (ObjectA[j].PosB.Pos.Y + ObjectAPos.Y), (ObjectA[j].PosB.Pos.Z + ObjectAPos.Z));
                        break;
                    case 2:
                        TempAPoint.Input((ObjectA[j].PosC.Pos.X + ObjectAPos.X), (ObjectA[j].PosC.Pos.Y + ObjectAPos.Y), (ObjectA[j].PosC.Pos.Z + ObjectAPos.Z));
                        break;
                    case 3:
                        TempAPoint.Input((ObjectA[j].PosD.Pos.X + ObjectAPos.X), (ObjectA[j].PosD.Pos.Y + ObjectAPos.Y), (ObjectA[j].PosD.Pos.Z + ObjectAPos.Z));
                        break;
                }
                ObjectAShadow.push_back(PhysicsEngine::DotPointToForce(TempAPoint, ObjectA[i].PlaneNorm, CenterPoint));

            }
        }

        for(long unsigned int j = 0; j < ObjectB.size(); j++){
            

            for(int n = 0; n < 4; n++){
                switch(n){
                    case 0:
                        TempBPoint.Input((ObjectB[j].PosA.Pos.X + ObjectBPos.X), (ObjectB[j].PosA.Pos.Y + ObjectBPos.Y), (ObjectB[j].PosA.Pos.Z + ObjectBPos.Z));
                        break;
                    case 1:
                        TempBPoint.Input((ObjectB[j].PosB.Pos.X + ObjectBPos.X), (ObjectB[j].PosB.Pos.Y + ObjectBPos.Y), (ObjectB[j].PosB.Pos.Z + ObjectBPos.Z));
                        break;
                    case 2:
                        TempBPoint.Input((ObjectB[j].PosC.Pos.X + ObjectBPos.X), (ObjectB[j].PosC.Pos.Y + ObjectBPos.Y), (ObjectB[j].PosC.Pos.Z + ObjectBPos.Z));
                        break;
                    case 3:
                        TempBPoint.Input((ObjectB[j].PosD.Pos.X + ObjectBPos.X), (ObjectB[j].PosD.Pos.Y + ObjectBPos.Y), (ObjectB[j].PosD.Pos.Z + ObjectBPos.Z));
                        break;
                }
                ObjectBShadow.push_back(PhysicsEngine::DotPointToForce(TempBPoint, ObjectB[i].PlaneNorm, CenterPoint));
            }
        }

        for(long unsigned int j =0; j < ObjectAShadow.size(); j++){
            if(ObjectAShadow[j] < ObjectAMin){
                ObjectAMin = ObjectAShadow[j];
            }

            if(ObjectAShadow[j] > ObjectAMin){
                ObjectAMax = ObjectAShadow[j];
            }
        }

        for(long unsigned int j =0; j < ObjectBShadow.size(); j++){
            if(ObjectBShadow[j] < ObjectBMin){
                ObjectBMin = ObjectBShadow[j];
            }

            if(ObjectBShadow[j] > ObjectBMin){
                ObjectBMax = ObjectBShadow[j];
            }
        }


        if(ObjectAMin >= ObjectBMin && ObjectAMin <= ObjectBMax){
            ObjectAShadow.clear();
            ObjectBShadow.clear();
        } else if (ObjectAMax >= ObjectBMin && ObjectAMax <= ObjectAMax){
            ObjectAShadow.clear();
            ObjectBShadow.clear();
        } else {
            // Know colission
            //Output.IsColision = false;
            return Output;
        }


    }

    Output.IsColision = true;
    Output.MovmentDirectionA = PhysicsEngine::MakeForceDirection(ObjectAPos, ObjectBPos);
    Output.MovmentDirectionB = PhysicsEngine::MakeForceDirection(ObjectBPos, ObjectAPos);
    //take the noral of each polly and dot product each point to it to make a "shadow"
    




    return Output;
}

std::vector<PlaneMinMax> PhysicsEngine::MinMaxFromQuads(std::vector<QuadPhysicsBody> Object, PhysicsPos ObjectPos){
    std::vector<PlaneMinMax> Output;

    PlaneMinMax TempMinMax;
    float XMin, YMin, ZMin, XMax, YMax, ZMax;

    float AX, BX, CX, DX;
    float AY, BY, CY, DY;
    float AZ, BZ, CZ, DZ;

    for(long unsigned int i = 0; i < Object.size(); i++){

        AX = Object[i].PosA.Pos.X + ObjectPos.X;
        AY = Object[i].PosA.Pos.Y + ObjectPos.Y;
        AZ = Object[i].PosA.Pos.Z + ObjectPos.Z;

        BX = Object[i].PosB.Pos.X + ObjectPos.X;
        BY = Object[i].PosB.Pos.Y + ObjectPos.Y;
        BZ = Object[i].PosB.Pos.Z + ObjectPos.Z;

        CX = Object[i].PosC.Pos.X + ObjectPos.X;
        CY = Object[i].PosC.Pos.Y + ObjectPos.Y;
        CZ = Object[i].PosC.Pos.Z + ObjectPos.Z;

        DX = Object[i].PosD.Pos.X + ObjectPos.X;
        DY = Object[i].PosD.Pos.Y + ObjectPos.Y;
        DZ = Object[i].PosD.Pos.Z + ObjectPos.Z;

        if(AX >= BX && AX >= CX && AX >= DX){
            XMax = AX;
        } else if(BX >= AX && BX >= CX && BX >= DX){
            XMax = BX;
        } else if(CX >= AX && CX >= BX && CX >= DX){
            XMax = CX;
        } else if(DX >= AX && DX >= CX && DX >= BX){
            XMax = DX;
        }

        if(AY >= BY && AY >= CY && AY >= DY){
            YMax = AY;
        } else if(BY >= AY && BY >= CY && BY >= DY){
            YMax = BY;
        } else if(CY >= AY && CY >= BY && CY >= DY){
            YMax = CY;
        } else if(DY >= AY && DY >= CY && DY >= BY){
            YMax = DY;
        }

        if(AZ >= BZ && AZ >= CZ && AZ >= DZ){
            ZMax = AZ;
        } else if(BZ >= AZ && BZ >= CZ && BZ >= DZ){
            ZMax = BZ;
        } else if(CZ >= AZ && CZ >= BZ && CZ >= DZ){
            ZMax = CZ;
        } else if(DZ >= AZ && DZ >= CZ && DZ >= BZ){
            ZMax = DZ;
        }


        if(AX <= BX && AX <= CX && AX <= DX){
            XMin = AX;
        } else if(BX <= AX && BX <= CX && BX <= DX){
            XMin = BX;
        } else if(CX <= AX && CX <= BX && CX <= DX){
            XMin = CX;
        } else if(DX <= AX && DX <= CX && DX <= BX){
            XMin = DX;
        }

        if(AY <= BY && AY <= CY && AY <= DY){
            YMin = AY;
        } else if(BY <= AY && BY <= CY && BY <= DY){
            YMin = BY;
        } else if(CY <= AY && CY <= BY && CY <= DY){
            YMin = CY;
        } else if(DY <= AY && DY <= CY && DY <= BY){
            YMin = DY;
        }

        if(AZ <= BZ && AZ <= CZ && AZ <= DZ){
            ZMin = AZ;
        } else if(BZ <= AZ && BZ <= CZ && BZ <= DZ){
            ZMin = BZ;
        } else if(CZ <= AZ && CZ <= BZ && CZ <= DZ){
            ZMin = CZ;
        } else if(DZ <= AZ && DZ <= CZ && DZ <= BZ){
            ZMin = DZ;
        }

        TempMinMax.Min.X = XMin;
        TempMinMax.Min.Y = YMin;
        TempMinMax.Min.Z = ZMin;

        TempMinMax.Max.X = XMax;
        TempMinMax.Max.Y = YMax;
        TempMinMax.Max.Z = ZMax;

        TempMinMax.Normal = Object[i].PlaneNorm;

        Output.push_back(TempMinMax);
    }



    return Output;
}

ColisionInfo PhysicsEngine::PointsToAABBColision(std::vector<QuadPhysicsBody> ObjectA, PhysicsPos ObjectAPos, std::vector<PlaneMinMax> ObjectB){
    ColisionInfo Output;
    Output.IsColision = false;

    PhysicsPos ComparePoint;

    bool XColision = false;
    bool YColision = false;
    bool ZColision = false;


    for(long unsigned int i = 0; i < ObjectA.size(); i++){
        for(long unsigned int j = 0; j < ObjectB.size(); j++){
            for(int n = 0; n < 4; n++){
                switch(n){
                    case 0:
                        ComparePoint.Input((ObjectA[i].PosA.Pos.X + ObjectAPos.X), (ObjectA[i].PosA.Pos.Y + ObjectAPos.Y), (ObjectA[i].PosA.Pos.Z + ObjectAPos.Z));
                        break;
                    case 1:
                        ComparePoint.Input((ObjectA[i].PosB.Pos.X + ObjectAPos.X), (ObjectA[i].PosB.Pos.Y + ObjectAPos.Y), (ObjectA[i].PosB.Pos.Z + ObjectAPos.Z));
                        break;
                    case 2:
                        ComparePoint.Input((ObjectA[i].PosC.Pos.X + ObjectAPos.X), (ObjectA[i].PosC.Pos.Y + ObjectAPos.Y), (ObjectA[i].PosC.Pos.Z + ObjectAPos.Z));
                        break;
                    case 3:
                        ComparePoint.Input((ObjectA[i].PosD.Pos.X + ObjectAPos.X), (ObjectA[i].PosD.Pos.Y + ObjectAPos.Y), (ObjectA[i].PosD.Pos.Z + ObjectAPos.Z));
                        break;
                }

                if(ComparePoint.X >= ObjectB[j].Min.X && ComparePoint.X <=ObjectB[j].Max.X){
                    XColision = true;
                } else {
                    XColision = false;
                }

                if(ComparePoint.Y >= ObjectB[j].Min.Y && ComparePoint.Y <=ObjectB[j].Max.Y){
                    YColision = true;
                } else {
                    YColision = false;
                }

                if(ComparePoint.Z >= ObjectB[j].Min.Z && ComparePoint.Z <=ObjectB[j].Max.Z){
                    ZColision = true;
                } else {
                    ZColision = false;
                }

                if(XColision && YColision && ZColision){
                    Output.IsColision = true;
                    Output.MovmentDirectionA = ObjectA[i].PlaneNorm;
                    Output.MovmentDirectionB = ObjectB[j].Normal;
                    return Output;
                }



            }
        } 
    }




    return Output;
}

void PhysicsEngine::AABBColision(std::vector<QuadPhysicsBody> &ObjectA, PhysicsPos &ObjectAPos, std::vector<QuadPhysicsBody> &ObjectB, PhysicsPos &ObjectBPos, ColisionInfo *Output){
    //ColisionInfo Temp;
    Output->IsColision = false;

    //QuadPhysicsBody *TempObject;

    bool XColision = false;
    bool YColision = false;
    bool ZColision = false;

    std::vector<PlaneMinMax> MinMaxA, MinMaxB;

    int IndexIntersectA, IntexIntersectB;
    bool isColision = false;

    PlaneMinMax TempMinMax;
    float XMin, YMin, ZMin, XMax, YMax, ZMax;
    XMax = -INFINITY;
    XMin = INFINITY;
    YMax = -INFINITY;
    YMin = INFINITY;
    ZMax = -INFINITY;
    ZMin = INFINITY;


    float AX, BX, CX, DX;
    float AY, BY, CY, DY;
    float AZ, BZ, CZ, DZ;


    for(long unsigned int i = 0; i < ObjectA.size(); i++){

        AX = ObjectA[i].PosA.Pos.X + ObjectAPos.X;
        AY = ObjectA[i].PosA.Pos.Y + ObjectAPos.Y;
        AZ = ObjectA[i].PosA.Pos.Z + ObjectAPos.Z;

        BX = ObjectA[i].PosB.Pos.X + ObjectAPos.X;
        BY = ObjectA[i].PosB.Pos.Y + ObjectAPos.Y;
        BZ = ObjectA[i].PosB.Pos.Z + ObjectAPos.Z;

        CX = ObjectA[i].PosC.Pos.X + ObjectAPos.X;
        CY = ObjectA[i].PosC.Pos.Y + ObjectAPos.Y;
        CZ = ObjectA[i].PosC.Pos.Z + ObjectAPos.Z;

        DX = ObjectA[i].PosD.Pos.X + ObjectAPos.X;
        DY = ObjectA[i].PosD.Pos.Y + ObjectAPos.Y;
        DZ = ObjectA[i].PosD.Pos.Z + ObjectAPos.Z;

        if(AX >= BX && AX >= CX && AX >= DX){
            XMax = AX;
        } else if(BX >= AX && BX >= CX && BX >= DX){
            XMax = BX;
        } else if(CX >= AX && CX >= BX && CX >= DX){
            XMax = CX;
        } else if(DX >= AX && DX >= CX && DX >= BX){
            XMax = DX;
        }

        if(AY >= BY && AY >= CY && AY >= DY){
            YMax = AY;
        } else if(BY >= AY && BY >= CY && BY >= DY){
            YMax = BY;
        } else if(CY >= AY && CY >= BY && CY >= DY){
            YMax = CY;
        } else if(DY >= AY && DY >= CY && DY >= BY){
            YMax = DY;
        }

        if(AZ >= BZ && AZ >= CZ && AZ >= DZ){
            ZMax = AZ;
        } else if(BZ >= AZ && BZ >= CZ && BZ >= DZ){
            ZMax = BZ;
        } else if(CZ >= AZ && CZ >= BZ && CZ >= DZ){
            ZMax = CZ;
        } else if(DZ >= AZ && DZ >= CZ && DZ >= BZ){
            ZMax = DZ;
        }

        if(AX <= BX && AX <= CX && AX <= DX){
            XMin = AX;
        } else if(BX <= AX && BX <= CX && BX <= DX){
            XMin = BX;
        } else if(CX <= AX && CX <= BX && CX <= DX){
            XMin = CX;
        } else if(DX <= AX && DX <= CX && DX <= BX){
            XMin = DX;
        }

        if(AY <= BY && AY <= CY && AY <= DY){
            YMin = AY;
        } else if(BY <= AY && BY <= CY && BY <= DY){
            YMin = BY;
        } else if(CY <= AY && CY <= BY && CY <= DY){
            YMin = CY;
        } else if(DY <= AY && DY <= CY && DY <= BY){
            YMin = DY;
        }

        if(AZ <= BZ && AZ <= CZ && AZ <= DZ){
            ZMin = AZ;
        } else if(BZ <= AZ && BZ <= CZ && BZ <= DZ){
            ZMin = BZ;
        } else if(CZ <= AZ && CZ <= BZ && CZ <= DZ){
            ZMin = CZ;
        } else if(DZ <= AZ && DZ <= CZ && DZ <= BZ){
            ZMin = DZ;
        }

        TempMinMax.Min.X = XMin;
        TempMinMax.Min.Y = YMin;
        TempMinMax.Min.Z = ZMin;

        TempMinMax.Max.X = XMax;
        TempMinMax.Max.Y = YMax;
        TempMinMax.Max.Z = ZMax;

        TempMinMax.Normal = ObjectA[i].PlaneNorm;

        MinMaxA.push_back(TempMinMax);

    }

    XMax = -INFINITY;
    XMin = INFINITY;
    YMax = -INFINITY;
    YMin = INFINITY;
    ZMax = -INFINITY;
    ZMin = INFINITY;

    for(long unsigned int i = 0; i < ObjectB.size(); i++){

        AX = ObjectB[i].PosA.Pos.X + ObjectBPos.X;
        AY = ObjectB[i].PosA.Pos.Y + ObjectBPos.Y;
        AZ = ObjectB[i].PosA.Pos.Z + ObjectBPos.Z;

        BX = ObjectB[i].PosB.Pos.X + ObjectBPos.X;
        BY = ObjectB[i].PosB.Pos.Y + ObjectBPos.Y;
        BZ = ObjectB[i].PosB.Pos.Z + ObjectBPos.Z;

        CX = ObjectB[i].PosC.Pos.X + ObjectBPos.X;
        CY = ObjectB[i].PosC.Pos.Y + ObjectBPos.Y;
        CZ = ObjectB[i].PosC.Pos.Z + ObjectBPos.Z;

        DX = ObjectB[i].PosD.Pos.X + ObjectBPos.X;
        DY = ObjectB[i].PosD.Pos.Y + ObjectBPos.Y;
        DZ = ObjectB[i].PosD.Pos.Z + ObjectBPos.Z;

        if(AX >= BX && AX >= CX && AX >= DX){
            XMax = AX;
        } else if(BX >= AX && BX >= CX && BX >= DX){
            XMax = BX;
        } else if(CX >= AX && CX >= BX && CX >= DX){
            XMax = CX;
        } else if(DX >= AX && DX >= CX && DX >= BX){
            XMax = DX;
        }

        if(AY >= BY && AY >= CY && AY >= DY){
            YMax = AY;
        } else if(BY >= AY && BY >= CY && BY >= DY){
            YMax = BY;
        } else if(CY >= AY && CY >= BY && CY >= DY){
            YMax = CY;
        } else if(DY >= AY && DY >= CY && DY >= BY){
            YMax = DY;
        }

        if(AZ >= BZ && AZ >= CZ && AZ >= DZ){
            ZMax = AZ;
        } else if(BZ >= AZ && BZ >= CZ && BZ >= DZ){
            ZMax = BZ;
        } else if(CZ >= AZ && CZ >= BZ && CZ >= DZ){
            ZMax = CZ;
        } else if(DZ >= AZ && DZ >= CZ && DZ >= BZ){
            ZMax = DZ;
        }


        if(AX <= BX && AX <= CX && AX <= DX){
            XMin = AX;
        } else if(BX <= AX && BX <= CX && BX <= DX){
            XMin = BX;
        } else if(CX <= AX && CX <= BX && CX <= DX){
            XMin = CX;
        } else if(DX <= AX && DX <= CX && DX <= BX){
            XMin = DX;
        }

        if(AY <= BY && AY <= CY && AY <= DY){
            YMin = AY;
        } else if(BY <= AY && BY <= CY && BY <= DY){
            YMin = BY;
        } else if(CY <= AY && CY <= BY && CY <= DY){
            YMin = CY;
        } else if(DY <= AY && DY <= CY && DY <= BY){
            YMin = DY;
        }

        if(AZ <= BZ && AZ <= CZ && AZ <= DZ){
            ZMin = AZ;
        } else if(BZ <= AZ && BZ <= CZ && BZ <= DZ){
            ZMin = BZ;
        } else if(CZ <= AZ && CZ <= BZ && CZ <= DZ){
            ZMin = CZ;
        } else if(DZ <= AZ && DZ <= CZ && DZ <= BZ){
            ZMin = DZ;
        }

        TempMinMax.Min.X = XMin;
        TempMinMax.Min.Y = YMin;
        TempMinMax.Min.Z = ZMin;

        TempMinMax.Max.X = XMax;
        TempMinMax.Max.Y = YMax;
        TempMinMax.Max.Z = ZMax;

        TempMinMax.Normal = ObjectA[i].PlaneNorm;

        MinMaxB.push_back(TempMinMax);
    }

    for(long unsigned int i = 0; i < MinMaxA.size(); i++){
        for(long unsigned int j = 0; j < MinMaxB.size(); j++){
            if((MinMaxA[i].Min.X <= MinMaxB[j].Min.X && MinMaxA[i].Max.X >= MinMaxB[j].Max.X) || (MinMaxA[i].Min.X >= MinMaxB[j].Min.X && MinMaxA[i].Max.X <= MinMaxB[j].Max.X)){
                XColision = true;
            } else {
                XColision = false;
            }
            if((MinMaxA[i].Min.Y <= MinMaxB[j].Min.Y && MinMaxA[i].Max.Y >= MinMaxB[j].Max.Y) || (MinMaxA[i].Min.Y >= MinMaxB[j].Min.Y && MinMaxA[i].Max.Y <= MinMaxB[j].Max.Y)){
                YColision = true;
            } else {
                YColision = false;
            }
            if((MinMaxA[i].Min.Z <= MinMaxB[j].Min.Z && MinMaxA[i].Max.Z >= MinMaxB[j].Max.Z) || (MinMaxA[i].Min.Z >= MinMaxB[j].Min.Z && MinMaxA[i].Max.Z <= MinMaxB[j].Max.Z)){
                ZColision = true;
            } else {
                ZColision = false;
            }

            if(XColision && ZColision && YColision){
                isColision = true;
                IndexIntersectA = i;
                IntexIntersectB = j;
                break;
            }
        }

        if(isColision){
            break;
        }
    }

    /*char TestChar = 'A';

    switch(TestChar){
            case 'B':
                std::cout << "B" << std::endl;
                break;
            case 'A':
                std::cout << "A" << std::endl;
                break;
    }*/

    Output->IsColision = isColision;
    Output->PastMovmentDirectionA = Output->MovmentDirectionA;
    Output->PastMovmentDirectionB = Output->MovmentDirectionB;
    if(isColision){
        Output->MovmentDirectionA = ObjectA[IndexIntersectA].PlaneNorm;
        Output->MovmentDirectionB = ObjectB[IntexIntersectB].PlaneNorm;
    } else {
        Output->MovmentDirectionA.X = 0.0f;
        Output->MovmentDirectionA.Y = 0.0f;
        Output->MovmentDirectionA.Z = 0.0f;

        Output->MovmentDirectionB.X = 0.0f;
        Output->MovmentDirectionB.Y = 0.0f;
        Output->MovmentDirectionB.Z = 0.0f;
    }

    Output->Force = 0.0f; // This should be a dynamicly calculated thing maybe 1 for each object

    //std::cout << IndexIntersectA << " | " << IntexIntersectB << std::endl;
    //std::cout << MinMaxA[IndexIntersectA].Min.X << " | " << MinMaxA[IndexIntersectA].Max.X << std::endl;
    //std::cout << MinMaxB[IntexIntersectB].Min.X << " | " << MinMaxB[IntexIntersectB].Max.X << std::endl;

    //std::cout << ObjectA[IndexIntersectA].PosA.X + ObjectAPos.X << std::endl;


    return;
}

void PhysicsEngine::SphearColison(PhysicsPos ObjectAPos, float ObjectASize, PhysicsPos ObjectBPos, float ObjectBSize, ColisionInfo *Output){
    //ColisionInfo Output;

    Output->IsColision = false;
    
    float DistAToB = std::sqrt((ObjectAPos.X - ObjectBPos.X)*(ObjectAPos.X - ObjectBPos.X) + (ObjectAPos.Y - ObjectBPos.Y)*(ObjectAPos.Y - ObjectBPos.Y) + (ObjectAPos.Z - ObjectBPos.Z)*(ObjectAPos.Z - ObjectBPos.Z));

    if(DistAToB < ObjectASize || DistAToB < ObjectBSize){
        Output->IsColision = true;
        Output->MovmentDirectionA = MakeForceDirection(ObjectAPos, ObjectBPos);
        Output->MovmentDirectionB = MakeForceDirection(ObjectBPos, ObjectAPos); 
    }

    return;
}

float PhysicsEngine::PointToPointDistance(PhysicsPos PointA, PhysicsPos PointB){
    float Distance = std::sqrt((PointA.X - PointB.X)*(PointA.X - PointB.X) + (PointA.Y - PointB.Y)*(PointA.Y - PointB.Y) + (PointA.Z - PointB.Z)*(PointA.Z - PointB.Z));

    return Distance;
}

float PhysicsEngine::DotPointToForce(PhysicsPos Point, ForceDirection Projection, PhysicsPos ProjectionPos){
    float Output;

    Output = ((Point.X * (Projection.X + ProjectionPos.X)) + (Point.Y * (Projection.Y + ProjectionPos.Y)) + (Point.Z * (Projection.Z + ProjectionPos.Z)));

    return Output;
}

PhysicsPos PhysicsEngine::Center2Point(PhysicsPos PointA, PhysicsPos PointB){
    PhysicsPos Output;
    Output.X = (PointA.X + PointB.X)/2.0f;
    Output.Y = (PointA.Y + PointB.Y)/2.0f;
    Output.Z = (PointA.Z + PointB.Z)/2.0f;

    return Output;

}

PhysicsPos PhysicsEngine::Center4Point(PhysicsPos *PointA, PhysicsPos *PointB, PhysicsPos *PointC, PhysicsPos *PointD){
    PhysicsPos Output;
    Output.X = (PointA->X + PointB->X + PointC->X + PointD->X)/4.0f;
    Output.Y = (PointA->Y + PointB->Y + PointC->Y + PointD->Y)/4.0f;
    Output.Z = (PointA->Z + PointB->Z + PointC->Z + PointD->Z)/4.0f;

    return Output;

}


ColisionInfo PhysicsEngine::SphearToPlane(SphearPhysicsBody ObjectA, QuadPhysicsBody ObjectB, PhysicsPos ObjectBPos) {
    ColisionInfo Output;
    QuadPhysicsBody TempQuad;

    TempQuad.PosA.Pos.X = ObjectB.PosA.Pos.X * ObjectBPos.X;
    TempQuad.PosA.Pos.X = ObjectB.PosA.Pos.X * ObjectBPos.X;
    TempQuad.PosA.Pos.X = ObjectB.PosA.Pos.X * ObjectBPos.X;

    TempQuad.PosA.Pos.Y = ObjectB.PosA.Pos.Y * ObjectBPos.Y;
    TempQuad.PosA.Pos.Y = ObjectB.PosA.Pos.Y * ObjectBPos.Y;
    TempQuad.PosA.Pos.Y = ObjectB.PosA.Pos.Y * ObjectBPos.Y;

    TempQuad.PosA.Pos.Z = ObjectB.PosA.Pos.Z * ObjectBPos.Z;
    TempQuad.PosA.Pos.Z = ObjectB.PosA.Pos.Z * ObjectBPos.Z;
    TempQuad.PosA.Pos.Z = ObjectB.PosA.Pos.Z * ObjectBPos.Z;

    TempQuad.PosB.Pos.X = ObjectB.PosB.Pos.X * ObjectBPos.X;
    TempQuad.PosB.Pos.X = ObjectB.PosB.Pos.X * ObjectBPos.X;
    TempQuad.PosB.Pos.X = ObjectB.PosB.Pos.X * ObjectBPos.X;

    TempQuad.PosB.Pos.Y = ObjectB.PosB.Pos.Y * ObjectBPos.Y;
    TempQuad.PosB.Pos.Y = ObjectB.PosB.Pos.Y * ObjectBPos.Y;
    TempQuad.PosB.Pos.Y = ObjectB.PosB.Pos.Y * ObjectBPos.Y;

    TempQuad.PosB.Pos.Z = ObjectB.PosB.Pos.Z * ObjectBPos.Z;
    TempQuad.PosB.Pos.Z = ObjectB.PosB.Pos.Z * ObjectBPos.Z;
    TempQuad.PosB.Pos.Z = ObjectB.PosB.Pos.Z * ObjectBPos.Z;

    TempQuad.PosC.Pos.X = ObjectB.PosC.Pos.X * ObjectBPos.X;
    TempQuad.PosC.Pos.X = ObjectB.PosC.Pos.X * ObjectBPos.X;
    TempQuad.PosC.Pos.X = ObjectB.PosC.Pos.X * ObjectBPos.X;

    TempQuad.PosC.Pos.Y = ObjectB.PosC.Pos.Y * ObjectBPos.Y;
    TempQuad.PosC.Pos.Y = ObjectB.PosC.Pos.Y * ObjectBPos.Y;
    TempQuad.PosC.Pos.Y = ObjectB.PosC.Pos.Y * ObjectBPos.Y;

    TempQuad.PosC.Pos.Z = ObjectB.PosC.Pos.Z * ObjectBPos.Z;
    TempQuad.PosC.Pos.Z = ObjectB.PosC.Pos.Z * ObjectBPos.Z;
    TempQuad.PosC.Pos.Z = ObjectB.PosC.Pos.Z * ObjectBPos.Z;

    TempQuad.PosD.Pos.X = ObjectB.PosD.Pos.X * ObjectBPos.X;
    TempQuad.PosD.Pos.X = ObjectB.PosD.Pos.X * ObjectBPos.X;
    TempQuad.PosD.Pos.X = ObjectB.PosD.Pos.X * ObjectBPos.X;

    TempQuad.PosD.Pos.Y = ObjectB.PosD.Pos.Y * ObjectBPos.Y;
    TempQuad.PosD.Pos.Y = ObjectB.PosD.Pos.Y * ObjectBPos.Y;
    TempQuad.PosD.Pos.Y = ObjectB.PosD.Pos.Y * ObjectBPos.Y;

    TempQuad.PosD.Pos.Z = ObjectB.PosD.Pos.Z * ObjectBPos.Z;
    TempQuad.PosD.Pos.Z = ObjectB.PosD.Pos.Z * ObjectBPos.Z;
    TempQuad.PosD.Pos.Z = ObjectB.PosD.Pos.Z * ObjectBPos.Z;



    // see if the sphear is inside the quad
    if(ObjectA.Radius > PhysicsEngine::PointToPointDistance(ObjectA.PosA.Pos, TempQuad.PosA.Pos)){
        Output.IsColision = true;
    } else if(ObjectA.Radius > PhysicsEngine::PointToPointDistance(ObjectA.PosA.Pos, TempQuad.PosB.Pos)){
        Output.IsColision = true;
    } else if(ObjectA.Radius > PhysicsEngine::PointToPointDistance(ObjectA.PosA.Pos, TempQuad.PosC.Pos)){
        Output.IsColision = true;
    } else if(ObjectA.Radius > PhysicsEngine::PointToPointDistance(ObjectA.PosA.Pos, TempQuad.PosD.Pos)){
        Output.IsColision = true;
    }

    // if it is then we can just return the normal of the quad
    // if it is not then we need to find the closest point on the quad to the sphear
    // then we can find the normal of the quad at that point
    // then we can find the normal of the sphear at that point
    // then we can find the force direction
    // then we can find the force
    // then we can return the output

    if(Output.IsColision){
        std::cout << "Colision" << std::endl;
        Output.MovmentDirectionA = ObjectB.PlaneNorm;
        Output.MovmentDirectionB = ObjectB.PlaneNorm;
    }

    return Output;
}

void PhysicsEngine::NormalizeForceDirection(ForceDirection ForceA, ForceDirection ForceB, ForceDirection *Output){
    ForceDirection TempA, TempB;
    float Temp;
    // For TempA
    Temp = std::sqrt(ForceA.X * ForceA.X + ForceA.Y * ForceA.Y + ForceA.Z * ForceA.Z);
    if(Temp != 0.){
        TempA.X = ForceA.X / Temp;
        TempA.Y = ForceA.Y / Temp;
        TempA.Z = ForceA.Z / Temp;

        // This is to give a weighted normal for averaging the forces;
        TempA.X = ForceA.X * ForceA.Speed;
        TempA.Y = ForceA.Y * ForceA.Speed;
        TempA.Z = ForceA.Z * ForceA.Speed;
    }

    // For TempB
    Temp = std::sqrt(ForceB.X * ForceB.X + ForceB.Y * ForceB.Y + ForceB.Z * ForceB.Z);
    if(Temp != 0.){
        TempB.X = ForceB.X / Temp;
        TempB.Y = ForceB.Y / Temp;
        TempB.Z = ForceB.Z / Temp;

        // This is to give a weighted normal for averaging the forces;
        TempB.X = ForceB.X * ForceB.Speed;
        TempB.Y = ForceB.Y * ForceB.Speed;
        TempB.Z = ForceB.Z * ForceB.Speed;
    }

    // For Output
    Output->X = (TempA.X + TempB.X);
    Output->Y = (TempA.Y + TempB.Y);
    Output->Z = (TempA.Z + TempB.Z);
    Temp = std::sqrt(Output->X * Output->X + Output->Y * Output->Y + Output->Z * Output->Z);
    if(Temp != 0.){
        Output->X /= Temp;
        Output->Y /= Temp;
        Output->Z /= Temp;
        Output->Speed = Temp;
    }


    return;
}


void PhysicsEngine::InternalUpdate(SimplePhysicsObject *Object ,std::vector<SimplePhysicsObject *> *Objects, unsigned long *Start, unsigned long *Stop) {
    //for(unsigned long i = 0; i < Objects->size(); i++){
    //    this->Objects[i]->Move(); // it should not need a peramiter passed to it unless to overide the defalut movment calculations so uwu
    //}
    //Object->Move();
}


void PhysicsEngine::ThreadSkelington(std::vector<SimplePhysicsObject *> *Objects, unsigned long *Start,unsigned long *End, bool *Run, bool *Kill) {
    // pls add
    while(!*Kill){
        if(*Run){
            for(unsigned int i = *Start; i <= *End; i++){
                this->InternalUpdate(Objects->at(i), Objects, Start, End);
            }
            //Once done
            *Run = false;
        }
        else {

            // Currently just a empty point

        }
    }
}

void PhysicsEngine::AddSimplePhysicsObject(SimplePhysicsObject NewObject) {
    //SimplePhysicsObject *Test = new SimplePhysicsObject(UniqueID,TypeID, BaseObject, IsPlayer);
    this->PhysicsObjects.push_back(NewObject);
    
    //this->PhysicsObjects.at(this->PhysicsObjects.size()-1)->EngineVectorPos = this->PhysicsObjects.size();

    //this->PhysicsObjects.at(this->PhysicsObjects.size()-1)->SetStarterPosition(BaseObject->GetPosAlt());


};

void PhysicsEngine::RemoveSimplePhysicsObject(unsigned long ObjectPOSID){
    PhysicsObjects.erase(PhysicsObjects.begin()+ObjectPOSID);
};


void PhysicsEngine::Update(FlatFoxPhysics::ForceDirection UserInput) {
    // For every Physics Object in the Game World(or just a limited type)
    if(m_ObjectPoolSize == 0.0f){
        m_ObjectPoolSize = PhysicsObjects.size()/m_ThreadLimit;
    }
    // This loop is going to be on the main phys thread just to insure the player get its input
    std::vector<SimplePhysicsObject>::iterator ob = this->PhysicsObjects.begin();
    for(; ob != this->PhysicsObjects.end(); ob++){
        // set up auto threading to the pool limit
        //(*ob)->AddAppliedForce(this->GetGravity());
       if((*ob).IsPlayer){
           std::cout << (*ob).Position.X << " | " << (*ob).Position.Y << " | " << (*ob).Position.Z << std::endl;
           (*ob).AddAppliedForce(UserInput);
       }
       // This is the prestaged movment
        (*ob).Move(); // This is to be removed once other threads care issued
    }
    // Dispatch object pools per thread and wait for them to finish

    bool Dispatched = false;
    unsigned long count;
    std::vector<bool>::iterator Thread = m_ThreadRunTrack.begin();
    for(; Thread != m_ThreadRunTrack.end(); Thread++){
        (*Thread) = true;
    }
    while(!Dispatched){
        Thread = m_ThreadRunTrack.begin();
        for(; Thread != m_ThreadRunTrack.end(); Thread++){
            if(!(*Thread)){
                count += 1;
            } else {
                // Lazy Count
                count = 0;
                //Testing Remove once done
                (*Thread) = false;
            }
            if(count == m_ThreadRunTrack.size()){
                Dispatched = true;
                count = 0;
            }
        }
    }

    ob = this->PhysicsObjects.begin();
    for(; ob != this->PhysicsObjects.end(); ob++){
        (*ob).ApplyMovedPosition();
    }
    // Quick Range check for interactinos

    // If colision only update futuer information for "owned" object

    //Once All Threads Are Done!!!!
    // Attempt to move any objects that need to be moved (applie all futuer positions)

    //Dispatch threads again?

    // Check if their are any Simpile colisions befor moving the object

    // If Simple Colision then Type Check the Objects for further instructions

    // Move Object and check for colision at new point (in the future you can take more than one time step)

}

void PhysicsEngine::SimpleThreadTest(){
    std::cout << "Ya the Simple Thread Teast Worked" << std::endl;
}