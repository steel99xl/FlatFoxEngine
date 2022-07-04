#pragma once
//#include "VertexBuffer.hpp"
//#include "VertexBufferLayout.hpp"
//#include "FlatFoxGL.hpp"
#include "Imports/glm/glm.hpp"
#include "Imports/glm/gtc/matrix_transform.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "Texture.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
//#include "Camera.hpp"

namespace FlatFoxObject {

    struct VertexPos {
        float X;
        float Y;
        float Z;

        VertexPos() {}

        ~VertexPos() {}

        void Input(float x, float y, float z) {
            X = x;
            Y = y;
            Z = z;
        }


    };

    struct VertexTexCord {
        float X;
        float Y;
    };

    struct Vertex {
        VertexPos Pos;
        VertexPos NormalPos;
        VertexTexCord TexCord;
        float TexID;
    };

    struct ColorFloat {
        float R;
        float G;
        float B;
    };

    struct SimpleLightInfo {
        // Note that the Light Struct defined in the BasicShaders set needs position data as well
        VertexPos lightDir; // might have to be limited to a range of -1 to 1
        ColorFloat ambient;
        ColorFloat diffuse;
        ColorFloat specular;

        float Angle;

        float Const;
        float Linear;
        float Quadratic;
    };

    struct SimpleMaterialInfo {
        ColorFloat ambient;
        ColorFloat diffuse;
        ColorFloat specular;
        float shininess;
    };

    enum ObjectOriginType {
        SimpleQuad, SimpleCube
    };

    struct ObjectQuadID {
        ObjectOriginType Type;
        unsigned int ID = 0; // yes its unsigned even tho it can easily go negative when calculating
        int GroupID;

        std::array<int, 24> DrawElementPoint;
        // In an effort to keep the physics self isolated this property will be provided by the physics engine
        std::array<int, 6> PhysicsElementPoint;
        std::array<int, 36> IndexBufferElement; // this is just used when deleting;

        ObjectQuadID() {
            for (int i = 0; i < IndexBufferElement.size(); i++) {
                IndexBufferElement[i] = -1;
                DrawElementPoint[i % DrawElementPoint.size()] = -1;
                PhysicsElementPoint[i % PhysicsElementPoint.size()] = -1;
            }
        }
    };


    class SimpleObject {
    public:
        enum FaceDir {
            F_UP, F_DOWN, F_EAST, F_WEST, F_NORTH, F_SOUTH, F_NONE
        };
        enum BufferType {
            StaticBuffer, DynamicBuffer
        };
    protected:
        int m_MaxQuadCount, m_MaxQuads, m_UsedQuads;


        SimpleMaterialInfo m_Material;
        SimpleLightInfo m_Light;

        // Vectors a used so seach instance of an SimpleObject can have (m_MaxQuads * 4) verticies and (m_MaxQuads * 6) 
        std::vector<Vertex> m_Verticies;
        unsigned int m_VerticiesMax;

        unsigned int m_PhysicsTypeID;

        std::vector<unsigned int> m_Indices;
        unsigned int m_IndicMax;
        unsigned int m_IndicOffset;

        float m_OldX, m_OldY, m_OldZ;
        // The position of the object
        float m_X, m_Y, m_Z;
        bool m_FuturePosition = false;
        float m_FutureX, m_FutureY, m_FutureZ;
        // The Previouse position;
        // The calculated rotation of point (0.0, 1.0, 0.0) from the origin of the object
        // This is just so the movment calculcatons can be similar to the camera
        float m_rX, m_rY, m_rZ;
        // Object color
        float m_R, m_G, m_B;
        // Light color if object is emitting light
        float m_LR, m_LG, m_LB;
        // The weight of the object vertexs in KG
        std::vector<float> m_Weights;
        float m_TotalWeight;

        float m_CurrentMovmentSpeed;
        VertexPos m_CurrentMovementDir;

        bool SimpleColision;

        SimpleObject::BufferType m_BufferType;


        std::unique_ptr<VertexArray> m_VAO;

        //Astd::unique_ptr<Texture> m_Texture;
        //Texture m_Texture;

        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<IndexBuffer> m_IBO;

        std::unique_ptr<Shader> m_ShadowShader;


        std::vector<ObjectQuadID> m_SubObjectIDList;

        //std::vector<FlatFoxPhysics::PhysicsPos> m_SPVertecxPositions;
        //std::vector<FlatFoxPhysics::PhysicsPos> m_SPVertecxNormlPositions;





        glm::vec3 Rotatex(glm::vec3 Start, float Angle);

        glm::vec3 Rotatey(glm::vec3 Start, float Angle);

        glm::vec3 Rotatez(glm::vec3 Start, float Angle);


    public:

        int ObjectPositionPastID, ObjectPositionID, ObjectPositionFutureID;

        SimpleObject();

        ~SimpleObject();

        unsigned int PhysicsObjectID; // this is to keepd treack of whick physics object is linked with each render object

        // if the buffer is a StaticBuffer then you only have to bind the buffer once outside the draw loop
        void Setup(int MaxQuads = 10000, SimpleObject::BufferType buffertype = DynamicBuffer);

        // This will be used my the shader to place the "object" in the world


        // Each object can store a texture, and each object will be able to bind a texture 
        // Depending how a quad is set 1 object can use Textures of other objects

        // This function needs to be rewriten to not take "target" from the user
        // Should append to the indexbuffer
        void Create2dQuad(float X, float Y, float Z, float AngleX, float AngleY, float AngleZ, float sizeX, float sizeY,
                          float Weight, float tX, float tY, float TX, float TY, float TextureID);

        void CreateCube(float X, float Y, float Z, float AngleX, float AngleY, float AngleZ, float SizeX, float SizeY,
                        float SizeZ, float Weight, float tX, float tY, float TX, float TY, float TexuteID);



        // A function that takes a vector of vertexes and appends it to this objects (so one object can draw all objects like it)
        // A function that takes in a vector of Vertex and replaces the current vector of Vertex
        // in each of these it should recalculate the indexbuffe object


        // Layout what i want functions to do

        // Add Quad to object ( so an object can be multiple quads)
        // Remove Quad from object
        // This would have to go back and remove stuff from the index buffer


        // Draw object
        // It is called paint for right now cause Renderer has a .Draw function
        // if all things drawn only read data, then the drawing could me moved to a seperate thread
        void Paint();

        void PaintShadow();


        inline std::vector<Vertex> GetVerticies() { return m_Verticies; }

        inline int GetVerticiesCount() { return m_UsedQuads * 4; }

        inline std::vector<unsigned int> GetIndices() { return m_Indices; }

        inline int GetIndicCount() { return (m_UsedQuads * 4) * 6; }

        inline int GetMaxQuadCound() { return m_MaxQuadCount; }

        inline int GetUsedQuads() { return m_UsedQuads; }

        inline std::vector<float> GetWeights() { return m_Weights; }

        inline std::vector<float> *GetWeightsPointer() { return &m_Weights; }

        inline SimpleMaterialInfo GetMaterialInfo() { return m_Material; }

        inline SimpleLightInfo GetLightInfo() { return m_Light; }

        inline void SetLightColor(float R, float G, float B) {
            m_LR = R;
            m_LG = G;
            m_LB = B;
        }

        // Cause why would any one want a negative PhysicsTypeID?
        inline void SetPhysicsTypeID(unsigned int TypeID) { m_PhysicsTypeID = TypeID; }

        inline unsigned int GetPhysicsTypeID() { return m_PhysicsTypeID; }

        inline glm::vec3 GetLightColor() { return glm::vec3(m_LR, m_LG, m_LB); }

        inline void SetPosition(float X, float Y, float Z) {
            m_OldX = m_X, m_OldY = m_Y, m_OldZ = m_Z, m_X = X;
            m_Y = Y;
            m_Z = Z;
        }

        inline void SetFuturePosition(float X, float Y, float Z) {
            m_FutureX = X;
            m_FutureY = Y;
            m_FutureZ = Z;
        }

        // this swaps the future position with the current postiton with no context for movment
        inline void SwapFuturePosition() {
            m_X = m_FutureX;
            m_Y = m_FutureY;
            m_Z = m_FutureZ;
        }

        inline void SetFuturePositionUpdate(bool enable) { m_FuturePosition = enable; }

        inline bool GetFuturePositionStatus() { return m_FuturePosition; }

        inline glm::vec3 GetPos() { return glm::vec3(m_X, m_Y, m_Z); }

        inline std::array<float,3> GetPosAlt(){std::array<float,3> Temp; Temp[0]=m_X;Temp[1]=m_Y;Temp[2]=m_Z; return Temp;}

        //inline FlatFoxPhysics::PhysicsPos GetPhysicsPos() {FlatFoxPhysics::PhysicsPos Output; Output.Input(m_X, m_Y, m_Z); return Output;}
        inline glm::vec3 GetPreviouPos() { return glm::vec3(m_OldX, m_OldY, m_OldZ); }
        //inline FlatFoxPhysics::PhysicsPos GetPreviouPhysicsPos() {FlatFoxPhysics::PhysicsPos Output; Output.Input(m_OldX, m_OldY, m_OldZ); return Output;}

        inline void SetColision(bool basic) { SimpleColision = basic; }

        inline bool GetColision() { return SimpleColision; }

        inline std::vector<ObjectQuadID> *GetObjectQuadID() { return &m_SubObjectIDList; }

        inline float GetTotalWeight() { return m_TotalWeight; }

        void SumAllWeights();

        //inline std::vector<FlatFoxPhysics::PhysicsPos> GetVertexPositions(){return m_SPVertecxPositions;};
        //inline std::vector<FlatFoxPhysics::PhysicsPos> GetVertexNormlPositions(){return m_SPVertecxNormlPositions;};

        //inline std::vector<FlatFoxPhysics::PhysicsPos> *GetVertexPositionsPointer(){return &m_SPVertecxPositions;};
        //inline std::vector<FlatFoxPhysics::PhysicsPos> *GetVertexNormlPositionsPointer(){return &m_SPVertecxNormlPositions;};

        void SetShader(const std::string &filePath);

        void FinishShader();

        std::vector<unsigned int> ExportShaders();

        void ImportShaders(std::vector<unsigned int> Import);

        // This removes the indevidual compiled shaders and only keeps the final output for the object
        void ClearShaderCache();

        void SetShadowShader(const std::string &filePath);

        void FinishShadowShader();

        std::vector<unsigned int> ExportShadowShaders();

        void ImportShadowShaders(std::vector<unsigned int> Import);

        // This removes the indevidual compiled shaders and only keeps the final output for the object
        void ClearShadowShaderCache();


        void SetDrawPos(glm::mat4 &Projection, glm::mat4 &View);

        void SetShadowPos(glm::mat4 &ShadowProjection, glm::mat4 &ShadowView);

        //void SetShadowMatrix(glm::mat4 &ShadowMatrix);

        void SetColor(float r, float g, float b, float a);

        void SetMaterial(SimpleMaterialInfo &Material);

        void MakeMaterial(float AmbientR, float AmbientG, float AmbientB, float DiffuseR, float DiffuseG, float DiffuseB,
                     float SpecularR, float SpecularG, float SpecularB, float Shininess);

        void MakeLight(float AmbientR, float AmbientG, float AmbientB, float DiffuseR, float DiffuseG, float DiffuseB,
                       float SpecularR, float SpecularG, float SpecularB, float LightDirX, float LightDirY,
                       float LightDirZ, float AngleSize, float Linear, float Quadratic);

        // This needs to match the number the shader expects
        void PreFillLights(int NumberOfLights);

        void SetLight(SimpleLightInfo lightInfo, glm::vec3 lightPos, glm::vec3 camPos, int LightNumber = 0);

        // Sets up the object to act as a source of light;
        // void SetLightEmission()

        void AddTexture(const std::string &filePath, unsigned int slot = 0);

        void SetTexture(unsigned int Texture, const std::string &UniformName);

        void SetFloatUniform(const std::string &UniformName, float data);

        void BindBufferData();

        void BindVertexBuffer();

        void BindIndexBuffer();

        // These will have to export the date used to make the physics equivilants...
        // Calculate or Set Bounding Cube

        // Calculate or set Bounding Sphear


        //inline std::unique_ptr<VertexArray> GetVertexArray() {return m_VAO;}
        //inline std::unique_ptr<IndexBuffer> GetIndexObject() {return m_IBO;}
        //inline std::unique_ptr<Shader> GetShader() {return m_Shader;}


    };

}