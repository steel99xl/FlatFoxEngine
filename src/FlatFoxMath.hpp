#pragma once
#include <cmath>
#include "glm/glm/glm.hpp"
// FlatFoxEngine "math" extention to define types shared through out the engine
namespace FlatFoxMath{

    struct Point{
        glm::vec3 Pos;
        double Weight;

        Point(double Weight = 0.0, glm::vec3 Pos = glm::vec3(0.0,0.0,0.0)) : Weight(Weight), Pos(Pos){

        }
        Point& operator=(const Point& P){
            Pos = P.Pos;
            Weight = P.Weight;
            return *this;
        }

        Point operator+(const Point& P)const {
            return Point(Weight + P.Weight, glm::vec3(Pos[0] + P.Pos[0], Pos[1] + P.Pos[1] , Pos[2] + P.Pos[2]));
        }

        Point operator-(const Point& P)const {
            return Point(Weight + P.Weight, glm::vec3(Pos[0] - P.Pos[0], Pos[1] - P.Pos[1] , Pos[2] - P.Pos[2]));
        }

        Point operator*(const Point& P)const {
            return Point(Weight + P.Weight, glm::vec3(Pos[0] * P.Pos[0], Pos[1] * P.Pos[1] , Pos[2] * P.Pos[2]));
        }

        Point operator/(const Point& P)const {
            return Point(Weight + P.Weight, glm::vec3(Pos[0] / P.Pos[0], Pos[1] / P.Pos[1] , Pos[2] / P.Pos[2]));
        }


        double Magnitude() const{
            double Temp = std::sqrt(Pos[0]*Pos[0] + Pos[1]*Pos[1] + Pos[2]*Pos[2]);
            return Temp;
        };

        Point Normalize() const{
            double Temp = std::sqrt(Pos[0]*Pos[0] + Pos[1]*Pos[1] + Pos[2]*Pos[2]);
            // Make sure vector does not exside 1 on all components
            double Input = (Pos[0]/Temp) + (Pos[1]/Temp) + (Pos[2]/Temp);
            if(Input > 1.0){
                Temp *= Input;
            }
            //
            if(Temp != 0.){
                return Point(Weight, glm::vec3(Pos[0]/Temp, Pos[1]/Temp, Pos[2]/Temp));
            }
            return Point();
        };

        double DistanceToPoint(const Point& P) const{
            double Distance = std::sqrt(Pos[0] - P.Pos[0] * (Pos[0] - P.Pos[0]) + (Pos[1] - P.Pos[1]) * (Pos[1]- P.Pos[1]) + (Pos[2] - P.Pos[2]) * (Pos[2] - P.Pos[2]));
            return Distance;
        };
    };

    struct Sphear{
        FlatFoxMath::Point Point;
        double Radius;

        Sphear(FlatFoxMath::Point Point, double Radius) : Point(Point), Radius(Radius){

        }
        
        Sphear& operator=(const Sphear& S){
            Point = S.Point;
            Radius = S.Radius;
            return *this;
        };

    };


    struct Triangle{
        FlatFoxMath::Point PointA;
        FlatFoxMath::Point PointB;
        FlatFoxMath::Point PointC;

        Triangle(FlatFoxMath::Point PointA, FlatFoxMath::Point PointB, FlatFoxMath::Point PointC) : PointA(PointA), PointB(PointB), PointC(PointC){

        }
        
        Triangle& operator=(const Triangle& Tri){
            PointA = Tri.PointA;
            PointB = Tri.PointB;
            PointC = Tri.PointC;
            return *this;
        };
    };

    struct Square{
        FlatFoxMath::Point PointA;
        FlatFoxMath::Point PointB;
        FlatFoxMath::Point PointC;
        FlatFoxMath::Point PointD;

        Square(FlatFoxMath::Point PointA ,FlatFoxMath::Point PointB ,FlatFoxMath::Point PointC ,FlatFoxMath::Point PointD) : PointA(PointA), PointB(PointB), PointC(PointC), PointD(PointD){

        }

        Square& operator=(const Square& Sqr){
            PointA = Sqr.PointA;
            PointB = Sqr.PointB;
            PointC = Sqr.PointC;
            PointD = Sqr.PointD;
            return *this;
        }

    };

    struct PointLight{
        glm::vec3 Pos; // mainly used for invisible sources
        glm::vec3 Ambient;
        glm::vec3 Diffuse;
        glm::vec3 Specular;

        float Const;
        float Linear;
        float Quadratic;

        PointLight(glm::vec3 Pos, glm::vec3 Ambient, glm::vec3 Diffuse, glm::vec3 Specular,float Const, float Linear, float Quadratic) : Pos(Pos), Ambient(Ambient), Diffuse(Diffuse), Specular(Specular), Const(Const), Linear(Linear), Quadratic(Quadratic){

        }

        PointLight& operator=(const PointLight& PointLight){
            Pos = PointLight.Pos;
            Ambient = PointLight.Ambient;
            Diffuse = PointLight.Diffuse;
            Specular = PointLight.Specular;

            Const = PointLight.Const;
            Linear = PointLight.Linear;
            Quadratic = PointLight.Quadratic;

            return *this;
        }

    };

    struct SimpleVisualMaterial{
        glm::vec3 Ambient;
        glm::vec3 Diffuse;
        glm::vec3 Specular;
        float Shininess;


        SimpleVisualMaterial(glm::vec3 Ambient, glm::vec3 Diffuse, glm::vec3 Specular, float Shininess) : Ambient(Ambient), Diffuse(Diffuse), Specular(Specular), Shininess(Shininess){

        }

        SimpleVisualMaterial& operator=(const SimpleVisualMaterial& SVM){
            Ambient = SVM.Ambient;
            Diffuse = SVM.Diffuse;
            Specular = SVM.Specular;
            Shininess = SVM.Shininess;
        
            return *this;
        }

    };

    struct VertexTexCord {
        float X;
        float Y;

        VertexTexCord(float X, float Y) : X(X), Y(Y) {}

        VertexTexCord& operator=(VertexTexCord T){
            X = T.X;
            Y = T.Y;

            return *this;
        }

    };

    


};

