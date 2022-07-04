#include "SimpleObject.hpp"
using namespace FlatFoxObject;

SimpleObject::SimpleObject(){
        

        //std::cout << "really.. " << std::endl;

}

SimpleObject::~SimpleObject(){

}

void SimpleObject::Setup(int MaxQuads, SimpleObject::BufferType buffertype){
        m_MaxQuadCount = MaxQuads;
        m_VerticiesMax = MaxQuads * 4;
        m_IndicOffset = 0;
        
        m_BufferType = buffertype;

        ObjectPositionPastID = -1;
        ObjectPositionID = -1;
        ObjectPositionFutureID = -1;
        //
        m_VAO = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>();
        m_Shader = std::make_unique<Shader>();
        m_ShadowShader = std::make_unique<Shader>();
        m_IBO = std::make_unique<IndexBuffer>();
        //m_Texture = std::make_unique<Texture>();

        SimpleColision = false;

        std::cout << m_MaxQuadCount << std::endl;

        if(m_BufferType == BufferType::StaticBuffer){
                m_IBO->MakeStaticBuffer(NULL, (m_MaxQuadCount*4)*6 );
                m_VertexBuffer->MakeStaticBuffer(NULL, sizeof(Vertex) * (m_MaxQuadCount*4));
        } else {
                m_IBO->MakeDynamicBuffer(NULL, (m_MaxQuadCount*4)*6 );
                m_VertexBuffer->MakeDynamicBuffer(NULL, sizeof(Vertex) * (m_MaxQuadCount*4));
        }
        
        //m_Shader->SetShader("assets/Shaders/MultiImg.shader");
        // This is just to set the basic rotation for the object in work space
        m_rX = 0.0f;
        m_rY = 1.0f;
        m_rZ = 0.0f;

        VertexBufferLayout layout;
        // This defines the layout
        // Position Data;
        layout.Push(3,"float");
        // Normolized Positon Data;
        layout.Push(3,"float");
        // Texure Map Data;
        layout.Push(2,"float");
        //Texture ID
        layout.Push(1,"float");
        m_VAO->AddBuffer(*m_VertexBuffer,layout);
}


glm::vec3 SimpleObject::Rotatex(glm::vec3 Start, float Angle){
    float Cosin, Sin;
    glm::vec3 Buffer;
    Cosin = cos(glm::radians(Angle));
    Sin = sin(glm::radians(Angle));

    Buffer.x = (1 * Start.x) + (0 * Start.y) + (0 * Start.z);
    Buffer.y = (0 * Start.x) + (Cosin * Start.y) + (-Sin * Start.z);
    Buffer.z = (0 * Start.x) + (Sin * Start.y) + (Cosin * Start.z);

    return Buffer;
}

glm::vec3 SimpleObject::Rotatey(glm::vec3 Start, float Angle){
    float Cosin, Sin;
    glm::vec3 Buffer;
    Cosin = cos(glm::radians(Angle));
    Sin = sin(glm::radians(Angle));

    Buffer.x = (Cosin * Start.x) + (0 * Start.y) + (Sin * Start.z);
    Buffer.y = (0 * Start.x) + (1 * Start.y) + (0 * Start.z);
    Buffer.z = (-Sin * Start.x) + (0 * Start.y) + (Cosin * Start.z);

    return Buffer;
}

glm::vec3 SimpleObject::Rotatez(glm::vec3 Start, float Angle){
        float Cosin, Sin;
        glm::vec3 Buffer;
        Cosin = cos(glm::radians(Angle));
        Sin = sin(glm::radians(Angle));

        Buffer.x = (Cosin * Start.x) + (-Sin * Start.y) + (0 * Start.z);
        Buffer.y = (Sin * Start.x) + (Cosin * Start.y) + (0 * Start.z);
        Buffer.z = (0 * Start.x) + (0 * Start.y) + (1 * Start.z);

        return Buffer;
}

void SimpleObject::Create2dQuad(float X, float Y, float Z, float AngleX, float AngleY, float AngleZ, float sizeX, float sizeY, float Weight, float tX, float tY, float TX, float TY, float TextureID){

        Vertex Temp;

        ObjectQuadID TempQuadID;




        glm::vec3 TempPosVec(X,Y,Z);
        std::vector<glm::vec3> SqrPt;
        SqrPt.push_back(glm::vec3 (-0.5*sizeX, -0.5*sizeY, 1.0f));
        SqrPt.push_back(glm::vec3 (0.5*sizeX, -0.5*sizeY, 1.0f));
        SqrPt.push_back(glm::vec3 (0.5*sizeX, 0.5*sizeY, 1.0f));
        SqrPt.push_back(glm::vec3 (-0.5*sizeX, 0.5*sizeY, 1.0f));

        glm::vec3 TempLightPosVec(0.0f,0.0f,1.0f);


        SqrPt[0] = Rotatex(SqrPt[0],AngleX);
        SqrPt[1] = Rotatex(SqrPt[1],AngleX);
        SqrPt[2] = Rotatex(SqrPt[2],AngleX);
        SqrPt[3] = Rotatex(SqrPt[3],AngleX);
        TempLightPosVec = Rotatex(TempLightPosVec, AngleX);


        SqrPt[0] = Rotatey(SqrPt[0],AngleY);
        SqrPt[1] = Rotatey(SqrPt[1],AngleY);
        SqrPt[2] = Rotatey(SqrPt[2],AngleY);
        SqrPt[3] = Rotatey(SqrPt[3],AngleY);
        TempLightPosVec = Rotatey(TempLightPosVec, AngleY);


        SqrPt[0] = Rotatez(SqrPt[0],AngleZ);
        SqrPt[1] = Rotatez(SqrPt[1],AngleZ);
        SqrPt[2] = Rotatez(SqrPt[2],AngleZ);
        SqrPt[3] = Rotatez(SqrPt[3],AngleZ);
        TempLightPosVec = Rotatez(TempLightPosVec, AngleZ);

        /* if(TempLightPosVec.x < 0.0f){
            TempLightPosVec.x *= -1.0f;
        }
        if(TempLightPosVec.y < 0.0f){
            TempLightPosVec.y *= -1.0f;
        }
        if(TempLightPosVec.z < 0.0f){
            TempLightPosVec.z *= -1.0f;
        }
        */

        for(int i = 0; i < 4; i++){
                Temp.Pos.Input(SqrPt[i].x + TempPosVec.x, SqrPt[i].y + TempPosVec.y, SqrPt[i].z + TempPosVec.z);
                Temp.NormalPos.Input(TempLightPosVec.x, TempLightPosVec.y, TempLightPosVec.z);
                switch(i){
                        case 0:
                                Temp.TexCord = {tX, tY};
                                break;
                        case 1:
                                Temp.TexCord = {TX, tY};
                                break;
                        case 2:
                                Temp.TexCord = {TX, TY};
                                break;
                        case 3:
                                Temp.TexCord = {tX, TY};
                                break;
                }
        //        Temp.TexCord = {tX, tY};
                Temp.TexID = TextureID;
                // This is done for odering reasons 
                TempQuadID.DrawElementPoint[i] = m_Verticies.size();
                m_Verticies.push_back(Temp);

                
                

                if(Weight == 0){
                        m_Weights.push_back(0);
                } else {
                        m_Weights.push_back(Weight/4);
                }
                
        }

        //std::cout << TempLightPosVec[0] << " | " << TempLightPosVec[1] << " | " << TempLightPosVec[2] << std::endl;



        TempQuadID.IndexBufferElement[0] = m_Indices.size();
        m_Indices.push_back(0 + m_IndicOffset);

        TempQuadID.IndexBufferElement[1] = m_Indices.size();
        m_Indices.push_back(1 + m_IndicOffset);
        
        TempQuadID.IndexBufferElement[3] = m_Indices.size();
        m_Indices.push_back(2 + m_IndicOffset);
        


        //indices[i + 3] =  2 + offset;
        //indices[i + 4] =  3 + offset;
        //indices[i + 5] =  0 + offset;

        TempQuadID.IndexBufferElement[4] = m_Indices.size();
        m_Indices.push_back(2 + m_IndicOffset);
        
        TempQuadID.IndexBufferElement[5] = m_Indices.size();
        m_Indices.push_back(3 + m_IndicOffset);
        
        TempQuadID.IndexBufferElement[6] = m_Indices.size();
        m_Indices.push_back(0 + m_IndicOffset);


        for(int i = 0; i < TempQuadID.IndexBufferElement.size(); i ++){
                TempQuadID.ID += TempQuadID.IndexBufferElement[i];
                TempQuadID.ID += TempQuadID.DrawElementPoint[i%TempQuadID.DrawElementPoint.size()];
        }



        m_SubObjectIDList.push_back(TempQuadID);


        m_IndicOffset += 4;

        m_UsedQuads += 1;

        //m_IndicCount += 6;

}

void SimpleObject::CreateCube(float X, float Y, float Z, float AngleX, float AngleY, float AngleZ, float SizeX, float SizeY, float SizeZ, float Weight, float tX, float tY, float TX, float TY, float TextureID){
        ObjectQuadID TempQuadID;
        // This hole function needs to be re-writen but for now im just adding this
        for(int i = 0; i < 24; i++){
                m_Weights.push_back(Weight/24);
        }

        glm::vec3 TempPosVec(X,Y,Z);
        std::vector<glm::vec3> CubePt;
        CubePt.push_back(glm::vec3 (-0.5*SizeX, -0.5*SizeY, 0.5* SizeZ));
        CubePt.push_back(glm::vec3 (0.5*SizeX, -0.5*SizeY, 0.5 * SizeZ));
        CubePt.push_back(glm::vec3 (0.5*SizeX, 0.5*SizeY, 0.5 * SizeZ));
        CubePt.push_back(glm::vec3 (-0.5*SizeX, 0.5*SizeY, 0.5 * SizeZ));


        CubePt.push_back(Rotatey(CubePt[0],180.0f));
        CubePt.push_back(Rotatey(CubePt[1],180.0f));
        CubePt.push_back(Rotatey(CubePt[2],180.0f));
        CubePt.push_back(Rotatey(CubePt[3],180.0f));

        Vertex Temp;


        // This shoudl be improved so its calculated at the face
        glm::vec3 CubeLightNorth(0.0f,0.0f,1.0f); // this is replicated in each vector for its face
        glm::vec3 CubeLightSouth(0.0f,0.0f,-1.0f); // this is replicated in each vector for its face
        glm::vec3 CubeLightEast(1.0f,0.0f,0.0f); // this is replicated in each vector for its face
        glm::vec3 CubeLightWest(-1.0f,0.0f,0.0f); // this is replicated in each vector for its face
        glm::vec3 CubeLightUp(0.0f,1.0f,0.0f); // this is replicated in each vector for its face
        glm::vec3 CubeLightDown(0.0f,-1.0f,0.0f); // this is replicated in each vector for its face

        //std::cout << print()

        //TempPosVec = Rotatex(TempPosVec, 45.0f);
        //TempPosVec = Rotatey(TempPosVec, 45.0f);

        CubePt[0] = Rotatex(CubePt[0], AngleX);
        CubePt[1] = Rotatex(CubePt[1], AngleX);
        CubePt[2] = Rotatex(CubePt[2], AngleX);
        CubePt[3] = Rotatex(CubePt[3], AngleX);
        CubePt[4] = Rotatex(CubePt[4], AngleX);
        CubePt[5] = Rotatex(CubePt[5], AngleX);
        CubePt[6] = Rotatex(CubePt[6], AngleX);
        CubePt[7] = Rotatex(CubePt[7], AngleX);


        CubePt[0] = Rotatey(CubePt[0], AngleY);
        CubePt[1] = Rotatey(CubePt[1], AngleY);
        CubePt[2] = Rotatey(CubePt[2], AngleY);
        CubePt[3] = Rotatey(CubePt[3], AngleY);
        CubePt[4] = Rotatey(CubePt[4], AngleY);
        CubePt[5] = Rotatey(CubePt[5], AngleY);
        CubePt[6] = Rotatey(CubePt[6], AngleY);
        CubePt[7] = Rotatey(CubePt[7], AngleY);


        CubePt[0] = Rotatez(CubePt[0], AngleZ);
        CubePt[1] = Rotatez(CubePt[1], AngleZ);
        CubePt[2] = Rotatez(CubePt[2], AngleZ);
        CubePt[3] = Rotatez(CubePt[3], AngleZ);
        CubePt[4] = Rotatez(CubePt[4], AngleZ);
        CubePt[5] = Rotatez(CubePt[5], AngleZ);
        CubePt[6] = Rotatez(CubePt[6], AngleZ);
        CubePt[7] = Rotatez(CubePt[7], AngleZ);


        CubeLightNorth = Rotatex(CubeLightNorth, AngleX);
        CubeLightSouth = Rotatex(CubeLightSouth, AngleX);
        CubeLightEast = Rotatex(CubeLightEast, AngleX);
        CubeLightWest = Rotatex(CubeLightWest, AngleX);
        CubeLightUp = Rotatex(CubeLightUp, AngleX);
        CubeLightDown = Rotatex(CubeLightDown, AngleX);


        CubeLightNorth = Rotatey(CubeLightNorth, AngleY);
        CubeLightSouth = Rotatey(CubeLightSouth, AngleY);
        CubeLightEast = Rotatey(CubeLightEast, AngleY);
        CubeLightWest = Rotatey(CubeLightWest, AngleY);
        CubeLightUp = Rotatey(CubeLightUp, AngleY);
        CubeLightDown = Rotatey(CubeLightDown, AngleY);


        CubeLightNorth = Rotatez(CubeLightNorth, AngleZ);
        CubeLightSouth = Rotatez(CubeLightSouth, AngleZ);
        CubeLightEast = Rotatez(CubeLightEast, AngleZ);
        CubeLightWest = Rotatez(CubeLightWest, AngleZ);
        CubeLightUp = Rotatez(CubeLightUp, AngleZ);
        CubeLightDown = Rotatez(CubeLightDown, AngleZ);





        // North quad
        Temp.Pos.Input(CubePt[0].x + TempPosVec.x, CubePt[0].y + TempPosVec.y, CubePt[0].z + TempPosVec.z);
        Temp.NormalPos.Input(CubeLightNorth.x, CubeLightNorth.y, CubeLightNorth.z);
        Temp.TexCord = {tX, tY};
        Temp.TexID = TextureID;

        TempQuadID.DrawElementPoint[0] = m_Verticies.size();
        m_Verticies.push_back(Temp);


        Temp.Pos.Input(CubePt[1].x + TempPosVec.x, CubePt[1].y + TempPosVec.y, CubePt[1].z + TempPosVec.z);
        Temp.NormalPos.Input(CubeLightNorth.x, CubeLightNorth.y, CubeLightNorth.z);
        Temp.TexCord = {TX, tY};
        Temp.TexID = TextureID;

        TempQuadID.DrawElementPoint[1] = m_Verticies.size();
        m_Verticies.push_back(Temp);

        Temp.Pos.Input(CubePt[2].x + TempPosVec.x, CubePt[2].y + TempPosVec.y, CubePt[2].z + TempPosVec.z);
        Temp.NormalPos.Input(CubeLightNorth.x, CubeLightNorth.y, CubeLightNorth.z);
        Temp.TexCord = {TX, TY};
        Temp.TexID = TextureID;

        TempQuadID.DrawElementPoint[2] = m_Verticies.size();
        m_Verticies.push_back(Temp);


        Temp.Pos.Input(CubePt[3].x + TempPosVec.x, CubePt[3].y + TempPosVec.y, CubePt[3].z + TempPosVec.z);
        Temp.NormalPos.Input(CubeLightNorth.x, CubeLightNorth.y, CubeLightNorth.z);
        Temp.TexCord = {tX, TY};
        Temp.TexID = TextureID;

        TempQuadID.DrawElementPoint[3] = m_Verticies.size();
        m_Verticies.push_back(Temp);

        TempQuadID.IndexBufferElement[0] = m_Indices.size();
        m_Indices.push_back(0 + m_IndicOffset);
        TempQuadID.IndexBufferElement[1] = m_Indices.size();
        m_Indices.push_back(1 + m_IndicOffset);
        TempQuadID.IndexBufferElement[2] = m_Indices.size();
        m_Indices.push_back(2 + m_IndicOffset);

        TempQuadID.IndexBufferElement[3] = m_Indices.size();
        m_Indices.push_back(2 + m_IndicOffset);
        TempQuadID.IndexBufferElement[4] = m_Indices.size();
        m_Indices.push_back(3 + m_IndicOffset);
        TempQuadID.IndexBufferElement[5] = m_Indices.size();
        m_Indices.push_back(0 + m_IndicOffset);

        m_IndicOffset += 4;

        m_UsedQuads += 1;

        // South Quad
        Temp.Pos.Input(CubePt[4].x + TempPosVec.x, CubePt[4].y + TempPosVec.y, CubePt[4].z + TempPosVec.z);
        Temp.NormalPos.Input(CubeLightSouth.x, CubeLightSouth.y, CubeLightSouth.z);
        Temp.TexCord = {tX, tY};
        Temp.TexID = TextureID;

        TempQuadID.DrawElementPoint[4] = m_Verticies.size();
        m_Verticies.push_back(Temp);


        Temp.Pos.Input(CubePt[5].x + TempPosVec.x, CubePt[5].y + TempPosVec.y, CubePt[5].z + TempPosVec.z);
        Temp.NormalPos.Input(CubeLightSouth.x, CubeLightSouth.y, CubeLightSouth.z);
        Temp.TexCord = {TX, tY};
        Temp.TexID = TextureID;

        TempQuadID.DrawElementPoint[5] = m_Verticies.size();
        m_Verticies.push_back(Temp);

        Temp.Pos.Input(CubePt[6].x + TempPosVec.x, CubePt[6].y + TempPosVec.y, CubePt[6].z + TempPosVec.z);
        Temp.NormalPos.Input(CubeLightSouth.x, CubeLightSouth.y, CubeLightSouth.z);
        Temp.TexCord = {TX, TY};
        Temp.TexID = TextureID;

        TempQuadID.DrawElementPoint[6] = m_Verticies.size();
        m_Verticies.push_back(Temp);


        Temp.Pos.Input(CubePt[7].x + TempPosVec.x, CubePt[7].y + TempPosVec.y, CubePt[7].z + TempPosVec.z);
        Temp.NormalPos.Input(CubeLightSouth.x, CubeLightSouth.y, CubeLightSouth.z);
        Temp.TexCord = {tX, TY};
        Temp.TexID = TextureID;

        TempQuadID.DrawElementPoint[7] = m_Verticies.size();
        m_Verticies.push_back(Temp);

        TempQuadID.IndexBufferElement[6] = m_Indices.size();
        m_Indices.push_back(0 + m_IndicOffset);
        TempQuadID.IndexBufferElement[7] = m_Indices.size();
        m_Indices.push_back(1 + m_IndicOffset);
        TempQuadID.IndexBufferElement[8] = m_Indices.size();
        m_Indices.push_back(2 + m_IndicOffset);

        TempQuadID.IndexBufferElement[9] = m_Indices.size();
        m_Indices.push_back(2 + m_IndicOffset);
        TempQuadID.IndexBufferElement[10] = m_Indices.size();
        m_Indices.push_back(3 + m_IndicOffset);
        TempQuadID.IndexBufferElement[11] = m_Indices.size();
        m_Indices.push_back(0 + m_IndicOffset);

        m_IndicOffset += 4;

        m_UsedQuads += 1;

        // East Quad
        Temp.Pos.Input(CubePt[1].x + TempPosVec.x, CubePt[1].y + TempPosVec.y, CubePt[1].z + TempPosVec.z);
        Temp.NormalPos.Input(CubeLightEast.x, CubeLightEast.y, CubeLightEast.z);
        Temp.TexCord = {tX, tY};
        Temp.TexID = TextureID;

        TempQuadID.DrawElementPoint[8] = m_Verticies.size();
        m_Verticies.push_back(Temp);


        Temp.Pos.Input(CubePt[4].x + TempPosVec.x, CubePt[4].y + TempPosVec.y, CubePt[4].z + TempPosVec.z);
        Temp.NormalPos.Input(CubeLightEast.x, CubeLightEast.y, CubeLightEast.z);
        Temp.TexCord = {TX, tY};
        Temp.TexID = TextureID;

        TempQuadID.DrawElementPoint[9] = m_Verticies.size();
        m_Verticies.push_back(Temp);

        Temp.Pos.Input(CubePt[7].x + TempPosVec.x, CubePt[7].y + TempPosVec.y, CubePt[7].z + TempPosVec.z);
        Temp.NormalPos.Input(CubeLightEast.x, CubeLightEast.y, CubeLightEast.z);
        Temp.TexCord = {TX, TY};
        Temp.TexID = TextureID;

        TempQuadID.DrawElementPoint[10] = m_Verticies.size();
        m_Verticies.push_back(Temp);


        Temp.Pos.Input(CubePt[2].x + TempPosVec.x, CubePt[2].y + TempPosVec.y, CubePt[2].z + TempPosVec.z);
        Temp.NormalPos.Input(CubeLightEast.x, CubeLightEast.y, CubeLightEast.z);
        Temp.TexCord = {tX, TY};
        Temp.TexID = TextureID;

        TempQuadID.DrawElementPoint[11] = m_Verticies.size();
        m_Verticies.push_back(Temp);

        TempQuadID.IndexBufferElement[12] = m_Indices.size();
        m_Indices.push_back(0 + m_IndicOffset);
        TempQuadID.IndexBufferElement[13] = m_Indices.size();
        m_Indices.push_back(1 + m_IndicOffset);
        TempQuadID.IndexBufferElement[14] = m_Indices.size();
        m_Indices.push_back(2 + m_IndicOffset);

        TempQuadID.IndexBufferElement[15] = m_Indices.size();
        m_Indices.push_back(2 + m_IndicOffset);
        TempQuadID.IndexBufferElement[16] = m_Indices.size();
        m_Indices.push_back(3 + m_IndicOffset);
        TempQuadID.IndexBufferElement[17] = m_Indices.size();
        m_Indices.push_back(0 + m_IndicOffset);

        m_IndicOffset += 4;

        m_UsedQuads += 1;

        // West Quad
        Temp.Pos.Input(CubePt[5].x + TempPosVec.x, CubePt[5].y + TempPosVec.y, CubePt[5].z + TempPosVec.z);
        Temp.NormalPos.Input(CubeLightWest.x, CubeLightWest.y, CubeLightWest.z);
        Temp.TexCord = {tX, tY};
        Temp.TexID = TextureID;

        TempQuadID.DrawElementPoint[12] = m_Verticies.size();
        m_Verticies.push_back(Temp);


        Temp.Pos.Input(CubePt[0].x + TempPosVec.x, CubePt[0].y + TempPosVec.y, CubePt[0].z + TempPosVec.z);
        Temp.NormalPos.Input(CubeLightWest.x, CubeLightWest.y, CubeLightWest.z);
        Temp.TexCord = {TX, tY};
        Temp.TexID = TextureID;

        TempQuadID.DrawElementPoint[13] = m_Verticies.size();
        m_Verticies.push_back(Temp);

        Temp.Pos.Input(CubePt[3].x + TempPosVec.x, CubePt[3].y + TempPosVec.y, CubePt[3].z + TempPosVec.z);
        Temp.NormalPos.Input(CubeLightWest.x, CubeLightWest.y, CubeLightWest.z);
        Temp.TexCord = {TX, TY};
        Temp.TexID = TextureID;

        TempQuadID.DrawElementPoint[14] = m_Verticies.size();
        m_Verticies.push_back(Temp);


        Temp.Pos.Input(CubePt[6].x + TempPosVec.x, CubePt[6].y + TempPosVec.y, CubePt[6].z + TempPosVec.z);
        Temp.NormalPos.Input(CubeLightWest.x, CubeLightWest.y, CubeLightWest.z);
        Temp.TexCord = {tX, TY};
        Temp.TexID = TextureID;

        TempQuadID.DrawElementPoint[15] = m_Verticies.size();
        m_Verticies.push_back(Temp);

        TempQuadID.IndexBufferElement[18] = m_Indices.size();
        m_Indices.push_back(0 + m_IndicOffset);
        TempQuadID.IndexBufferElement[19] = m_Indices.size();
        m_Indices.push_back(1 + m_IndicOffset);
        TempQuadID.IndexBufferElement[20] = m_Indices.size();
        m_Indices.push_back(2 + m_IndicOffset);

        TempQuadID.IndexBufferElement[21] = m_Indices.size();
        m_Indices.push_back(2 + m_IndicOffset);
        TempQuadID.IndexBufferElement[22] = m_Indices.size();
        m_Indices.push_back(3 + m_IndicOffset);
        TempQuadID.IndexBufferElement[23] = m_Indices.size();
        m_Indices.push_back(0 + m_IndicOffset);

        m_IndicOffset += 4;

        m_UsedQuads += 1;

        // "UP" Quad
        Temp.Pos.Input(CubePt[3].x + TempPosVec.x, CubePt[3].y + TempPosVec.y, CubePt[3].z + TempPosVec.z);
        Temp.NormalPos.Input(CubeLightUp.x, CubeLightUp.y, CubeLightUp.z);
        Temp.TexCord = {tX, tY};
        Temp.TexID = TextureID;

        TempQuadID.DrawElementPoint[16] = m_Verticies.size();
        m_Verticies.push_back(Temp);


        Temp.Pos.Input(CubePt[2].x + TempPosVec.x, CubePt[2].y + TempPosVec.y, CubePt[2].z + TempPosVec.z);
        Temp.NormalPos.Input(CubeLightUp.x, CubeLightUp.y, CubeLightUp.z);
        Temp.TexCord = {TX, tY};
        Temp.TexID = TextureID;

        TempQuadID.DrawElementPoint[17] = m_Verticies.size();
        m_Verticies.push_back(Temp);

        Temp.Pos.Input(CubePt[7].x + TempPosVec.x, CubePt[7].y + TempPosVec.y, CubePt[7].z + TempPosVec.z);
        Temp.NormalPos.Input(CubeLightUp.x, CubeLightUp.y, CubeLightUp.z);
        Temp.TexCord = {TX, TY};
        Temp.TexID = TextureID;

        TempQuadID.DrawElementPoint[18] = m_Verticies.size();
        m_Verticies.push_back(Temp);


        Temp.Pos.Input(CubePt[6].x + TempPosVec.x, CubePt[6].y + TempPosVec.y, CubePt[6].z + TempPosVec.z);
        Temp.NormalPos.Input(CubeLightUp.x, CubeLightUp.y, CubeLightUp.z);
        Temp.TexCord = {tX, TY};
        Temp.TexID = TextureID;

        TempQuadID.DrawElementPoint[19] = m_Verticies.size();
        m_Verticies.push_back(Temp);

        TempQuadID.IndexBufferElement[24] = m_Indices.size();
        m_Indices.push_back(0 + m_IndicOffset);
        TempQuadID.IndexBufferElement[25] = m_Indices.size();
        m_Indices.push_back(1 + m_IndicOffset);
        TempQuadID.IndexBufferElement[26] = m_Indices.size();
        m_Indices.push_back(2 + m_IndicOffset);

        TempQuadID.IndexBufferElement[27] = m_Indices.size();
        m_Indices.push_back(2 + m_IndicOffset);
        TempQuadID.IndexBufferElement[28] = m_Indices.size();
        m_Indices.push_back(3 + m_IndicOffset);
        TempQuadID.IndexBufferElement[29] = m_Indices.size();
        m_Indices.push_back(0 + m_IndicOffset);

        m_IndicOffset += 4;

        m_UsedQuads += 1;


        // "Down Quad"
        Temp.Pos.Input(CubePt[5].x + TempPosVec.x, CubePt[5].y + TempPosVec.y, CubePt[5].z + TempPosVec.z);
        Temp.NormalPos.Input(CubeLightDown.x, CubeLightDown.y, CubeLightDown.z);
        Temp.TexCord = {tX, tY};
        Temp.TexID = TextureID;

        TempQuadID.DrawElementPoint[20] = m_Verticies.size();
        m_Verticies.push_back(Temp);


        Temp.Pos.Input(CubePt[4].x + TempPosVec.x, CubePt[4].y + TempPosVec.y, CubePt[4].z + TempPosVec.z);
        Temp.NormalPos.Input(CubeLightDown.x, CubeLightDown.y, CubeLightDown.z);
        Temp.TexCord = {TX, tY};
        Temp.TexID = TextureID;

        TempQuadID.DrawElementPoint[21] = m_Verticies.size();
        m_Verticies.push_back(Temp);

        Temp.Pos.Input(CubePt[1].x + TempPosVec.x, CubePt[1].y + TempPosVec.y, CubePt[1].z + TempPosVec.z);
        Temp.NormalPos.Input(CubeLightDown.x, CubeLightDown.y, CubeLightDown.z);
        Temp.TexCord = {TX, TY};
        Temp.TexID = TextureID;

        TempQuadID.DrawElementPoint[22] = m_Verticies.size();
        m_Verticies.push_back(Temp);


        Temp.Pos.Input(CubePt[0].x + TempPosVec.x, CubePt[0].y + TempPosVec.y, CubePt[0].z + TempPosVec.z);
        Temp.NormalPos.Input(CubeLightDown.x, CubeLightDown.y, CubeLightDown.z);
        Temp.TexCord = {tX, TY};
        Temp.TexID = TextureID;

        TempQuadID.DrawElementPoint[23] = m_Verticies.size();
        m_Verticies.push_back(Temp);

        TempQuadID.IndexBufferElement[30] = m_Indices.size();
        m_Indices.push_back(0 + m_IndicOffset);
        TempQuadID.IndexBufferElement[31] = m_Indices.size();
        m_Indices.push_back(1 + m_IndicOffset);
        TempQuadID.IndexBufferElement[32] = m_Indices.size();
        m_Indices.push_back(2 + m_IndicOffset);

        TempQuadID.IndexBufferElement[33] = m_Indices.size();
        m_Indices.push_back(2 + m_IndicOffset);
        TempQuadID.IndexBufferElement[34] = m_Indices.size();
        m_Indices.push_back(3 + m_IndicOffset);
        TempQuadID.IndexBufferElement[35] = m_Indices.size();
        m_Indices.push_back(0 + m_IndicOffset);


        for(int i = 0; i < TempQuadID.IndexBufferElement.size(); i ++){
                TempQuadID.ID += TempQuadID.IndexBufferElement[i];
                TempQuadID.ID += TempQuadID.DrawElementPoint[i%TempQuadID.DrawElementPoint.size()];
        }



        m_SubObjectIDList.push_back(TempQuadID);

        m_IndicOffset += 4;

        m_UsedQuads += 1;
}

void SimpleObject::Paint(){
        Renderer renderer;

        renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
}

void SimpleObject::PaintShadow(){
        Renderer renderer;

        renderer.Draw(*m_VAO, *m_IBO, *m_ShadowShader);
}


void SimpleObject::SumAllWeights(){
        m_TotalWeight = 0.0f;
        for(int i = 0; i < m_Weights.size(); i++){
                m_TotalWeight += m_Weights[i];
        }
}

// Oh boy the physics engien is creeping in to the object
/*
void SimpleObject::GenerateSimplePhysicsInfo() {
    FlatFoxPhysics::PhysicsPos PosTemp, NormlTemp;
    m_SPVertecxPositions.clear();
    m_SPVertecxNormlPositions.clear();
    for(long unsigned int i = 0; i < m_Verticies.size(); i++){
        PosTemp.X = m_Verticies[i].Pos.X;
        PosTemp.Y = m_Verticies[i].Pos.Y;
        PosTemp.Z = m_Verticies[i].Pos.Z;

        NormlTemp.X = m_Verticies[i].NormalPos.X;
        NormlTemp.Y = m_Verticies[i].NormalPos.Y;
        NormlTemp.Z = m_Verticies[i].NormalPos.Z;

        m_SPVertecxPositions.push_back(PosTemp);
        m_SPVertecxNormlPositions.push_back(NormlTemp);
    }
}
 */

void SimpleObject::SetShader(const std::string &filePath){
        m_Shader->SetShader(filePath);

        // this is just for testing
        //int samplers[] = {0, 1, 2};
        //int samplers[] = {0};
        //m_Shader->SetUniform1iv("u_Textures", 3, samplers);
        //m_Shader->SetUniform1iv("u_Textures", 3, samplers);
        //m_Shader->SetUniform1i("u_Texture", 0);
        //m_Shader->SetUniform1i("u_Texture0", 1);
        //                      u_Texture0
        //m_Shader->SetUniform1i("u_Texture1", 2);
        //m_Shader->SetUniform1i("u_Texture2", 3);


}

void SimpleObject::FinishShader(){
        m_Shader->Finish();
}

std::vector<unsigned int> SimpleObject::ExportShaders(){
        return m_Shader->GetCompiledShaders();
}

void SimpleObject::ImportShaders(std::vector<unsigned int> Import){
        m_Shader->SetCompiledShaders(Import);
}

void SimpleObject::ClearShaderCache(){
        m_Shader->ClearCompiledShaders();
}
//
void SimpleObject::SetShadowShader(const std::string &filePath){
        m_ShadowShader->SetShader(filePath);
}

void SimpleObject::FinishShadowShader(){
        m_ShadowShader->Finish();
}

std::vector<unsigned int> SimpleObject::ExportShadowShaders(){
        return m_ShadowShader->GetCompiledShaders();
}

void SimpleObject::ImportShadowShaders(std::vector<unsigned int> Import){
        m_ShadowShader->SetCompiledShaders(Import);
}

void SimpleObject::ClearShadowShaderCache(){
        m_ShadowShader->ClearCompiledShaders();
}



void SimpleObject::SetDrawPos(glm::mat4 &Projection, glm::mat4 &View){
        glm::mat4 ModlePos = glm::translate(glm::mat4(1.0f), glm::vec3(m_X,m_Y,m_Z));
        m_Shader->Bind();
        m_Shader->SetUniformMat4f("Modle", ModlePos);
        m_Shader->SetUniformMat4f("View", View);
        m_Shader->SetUniformMat4f("Projection", Projection);
}

void SimpleObject::SetShadowPos(glm::mat4 &ShadowProjection, glm::mat4 &ShadowView){
        glm::mat4 ModlePos = glm::translate(glm::mat4(1.0f), glm::vec3(m_X, m_Y, m_Z));
        m_ShadowShader->Bind();
        m_ShadowShader->SetUniformMat4f("SModle", ModlePos);
        m_ShadowShader->SetUniformMat4f("SView", ShadowView);
        m_ShadowShader->SetUniformMat4f("SProjection", ShadowProjection);
        // This is just to pass the light information to the main rendering shader;
        m_Shader->Bind();
        m_Shader->SetUniformMat4f("LightView", ShadowView);
        m_Shader->SetUniformMat4f("LightProjection", ShadowProjection);
}


void SimpleObject::SetColor(float r, float g, float b, float a){
        m_R = r;
        m_G = g;
        m_B = b;
        m_Shader->Bind();
        m_Shader->SetUniform4f("u_Color", r, g, b, a);
}

void SimpleObject::SetMaterial(SimpleMaterialInfo &Material){
    m_Shader->Bind();

    m_Shader->SetUniform3f("u_Material.ambient", Material.ambient.R, Material.ambient.G, Material.ambient.B);
    m_Shader->SetUniform3f("u_Material.diffuse", Material.diffuse.R, Material.diffuse.G, Material.diffuse.B);
    m_Shader->SetUniform3f("u_Material.specular", Material.specular.R, Material.specular.G, Material.specular.B);
    m_Shader->SetUniform1f("u_Material.shininess", Material.shininess);
}

void SimpleObject::MakeMaterial(float AmbientR, float AmbientG, float AmbientB, float DiffuseR, float DiffuseG, float DiffuseB, float SpecularR, float SpecularG, float SpecularB, float Shininess){
        m_Material.ambient.R = AmbientR;
        m_Material.ambient.G = AmbientG;
        m_Material.ambient.B = AmbientB;

        m_Material.diffuse.R = DiffuseR;
        m_Material.diffuse.G = DiffuseG;
        m_Material.diffuse.B = DiffuseB;

        m_Material.specular.R = SpecularR;
        m_Material.specular.G = SpecularG;
        m_Material.specular.B = SpecularB;

        m_Material.shininess = Shininess;
}

void SimpleObject::MakeLight(float AmbientR, float AmbientG, float AmbientB, float DiffuseR, float DiffuseG, float DiffuseB, float SpecularR, float SpecularG, float SpecularB, float LightDirX, float LightDirY, float LightDirZ, float AngleSize, float Linear, float Quadratic){
        m_Light.ambient.R = AmbientR;
        m_Light.ambient.G = AmbientG;
        m_Light.ambient.B = AmbientB;

        m_Light.diffuse.R = DiffuseR;
        m_Light.diffuse.G = DiffuseG;
        m_Light.diffuse.B = DiffuseB;

        m_Light.specular.R = SpecularR;
        m_Light.specular.G = SpecularG;
        m_Light.specular.B = SpecularB;

        m_Light.Angle = glm::cos(glm::radians(AngleSize));

        m_Light.Const = 1.0f;
        m_Light.Linear = Linear;
        m_Light.Quadratic = Quadratic;

}


void SimpleObject::PreFillLights(int NumberOfLights){
        glm::vec3 lightPos = glm::vec3(0.0f,0.0f,0.0f);

        SimpleLightInfo lightInfo;

        lightInfo.lightDir.X = 0.0f;
        lightInfo.lightDir.Y = 0.0f;
        lightInfo.lightDir.Z = 0.0f;

        lightInfo.ambient.R = 0.0f;
        lightInfo.ambient.G = 0.0f;
        lightInfo.ambient.B = 0.0f;

        lightInfo.diffuse.R = 0.0f;
        lightInfo.diffuse.G = 0.0f;
        lightInfo.diffuse.B = 0.0f;

        lightInfo.specular.R = 0.0f;
        lightInfo.specular.G = 0.0f;
        lightInfo.specular.B = 0.0f;

        lightInfo.Angle = 12.5f;

        lightInfo.Quadratic = 0.0f;
        lightInfo.Const = 1.0f;
        lightInfo.Linear = 0.0f;

        std::string Number;

        m_Shader->Bind();

        for(int i = 0; i < NumberOfLights; i++){
                Number = std::to_string(i);

                m_Shader->SetUniform3f("lights["+Number+"].position", lightPos.x, lightPos.y, lightPos.z);
                m_Shader->SetUniform3f("lights["+Number+"].lightPoint", lightInfo.lightDir.X, lightInfo.lightDir.Y, lightInfo.lightDir.Z);
                m_Shader->SetUniform3f("lights["+Number+"].ambient", lightInfo.ambient.R, lightInfo.ambient.G, lightInfo.ambient.B);
                m_Shader->SetUniform3f("lights["+Number+"].diffuse", lightInfo.diffuse.R, lightInfo.diffuse.G, lightInfo.diffuse.B);
                m_Shader->SetUniform3f("lights["+Number+"].specular", lightInfo.specular.R, lightInfo.specular.G, lightInfo.specular.B);
                m_Shader->SetUniform1f("lights["+Number+"].Asize", glm::cos(glm::radians(lightInfo.Angle)));
                m_Shader->SetUniform1f("lights["+Number+"].constant", lightInfo.Const);
                m_Shader->SetUniform1f("lights["+Number+"].linear", lightInfo.Linear);
                m_Shader->SetUniform1f("lights["+Number+"].quadratic", lightInfo.Quadratic);
        }
}

void SimpleObject::SetLight(SimpleLightInfo lightInfo, glm::vec3 lightPos, glm::vec3 camPos, int LightNumber){
        std::string Number = std::to_string(LightNumber);
        m_Shader->Bind();
        m_Shader->SetUniform3f("lights["+Number+"].position", lightPos.x, lightPos.y, lightPos.z);
        m_Shader->SetUniform3f("lights["+Number+"].lightPoint", lightInfo.lightDir.X, lightInfo.lightDir.Y, lightInfo.lightDir.Z);
        m_Shader->SetUniform3f("lights["+Number+"].ambient", lightInfo.ambient.R, lightInfo.ambient.G, lightInfo.ambient.B);
        m_Shader->SetUniform3f("lights["+Number+"].diffuse", lightInfo.diffuse.R, lightInfo.diffuse.G, lightInfo.diffuse.B);
        m_Shader->SetUniform3f("lights["+Number+"].specular", lightInfo.specular.R, lightInfo.specular.G, lightInfo.specular.B);
        m_Shader->SetUniform1f("lights["+Number+"].Asize", glm::cos(glm::radians(lightInfo.Angle)));
        m_Shader->SetUniform1f("lights["+Number+"].constant", lightInfo.Const);
        m_Shader->SetUniform1f("lights["+Number+"].linear", lightInfo.Linear);
        m_Shader->SetUniform1f("lights["+Number+"].quadratic", lightInfo.Quadratic);
        m_Shader->SetUniform3f("u_camPos", camPos.x, camPos.y, camPos.z);

}

void SimpleObject::AddTexture(const std::string &filePath, unsigned int slot){
        //m_Texture.LoadTexture(filePath, slot);

        //m_Texture.Bind(slot);
}

void SimpleObject::SetTexture(unsigned int Texture, const std::string &UniformName){
        m_Shader->Bind();
        m_Shader->SetUniform1i(UniformName, Texture);
}

void SimpleObject::SetFloatUniform(const std::string &UniformName, float data){
        m_Shader->Bind();
        m_Shader->SetUniform1f(UniformName, data);
}

void SimpleObject::BindBufferData(){
        m_VertexBuffer->Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, (m_UsedQuads*4) * sizeof(Vertex), m_Verticies.data());

        m_IBO->Bind();
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, (m_UsedQuads*4)*6, m_Indices.data());
}

void SimpleObject::BindVertexBuffer(){
        m_VertexBuffer->Bind();
}

void SimpleObject::BindIndexBuffer(){
        m_IBO->Bind();
}
