// Uses FrameVertex.shader
#Shader fragment
#version 330 core
struct Size{
    float width;
    float height;
};

layout(location = 0) out vec4 Finalcolor;

in vec2 v_TexCord;

uniform sampler2D u_Texture;

void main(){
    

    Finalcolor = texture(u_Texture, v_TexCord);

    //Finalcolor = vec4(vec3(1.0f) - vec3(texture(u_Texture, v_TexCord)),1.0f);
    
}