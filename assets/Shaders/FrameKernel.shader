#Shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normalizedpos;
layout(location = 2) in vec2 texCord;
layout(location = 3) in float texIndex;

out vec2 v_TexCord;


void main(){
gl_Position = vec4(position, 1.0);
v_TexCord  = texCord;
}



#Shader fragment
#version 330 core
struct Size{
    float width;
    float height;
};

layout(location = 0) out vec4 Finalcolor;

in vec2 v_TexCord;

uniform sampler2D u_Texture;

uniform Size u_Size;


float offsetX = 1.0f / u_Size.width;
float offsetY = 1.0f / u_Size.height;

vec2 offsets[9] = vec2[](
    vec2(-offsetX, offsetY), vec2(0.0f, offsetY), vec2(offsetX, offsetY),
    vec2(-offsetX, 0.0f),   vec2(0.0f, 0.0f),   vec2(offsetX, 0.0f),
    vec2(-offsetX, -offsetY), vec2(0.0f, -offsetY), vec2(offsetX, -offsetY)
);

float kernel[9] = float[](
    1,1,1,
    1,-7.2,1,
    1,1,1
);

void main(){

    vec3 color = vec3(0.0f);

    for(int i = 0; i < 9; i++){
        color += vec3(texture(u_Texture, v_TexCord + offsets[i] )) * kernel[i];
    }

    Finalcolor = vec4(color,1.0f);

    //Finalcolor = texture(u_Texture, v_TexCord);

    //Finalcolor = vec4(vec3(1.0f) - vec3(texture(u_Texture, v_TexCord)),1.0f);
    
}