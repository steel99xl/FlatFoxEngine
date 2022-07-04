#Shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCord;
layout(location = 2) in float texIndex;

out vec2 v_TexCord;
out float v_TexIndex;
uniform mat4 u_MVP;

void main(){
gl_Position = u_MVP * position;
v_TexCord  = texCord;
v_TexIndex = texIndex;
}



#Shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCord;
in float v_TexIndex;

uniform vec4 u_Color;

//uniform sampler2D u_Texture;
void main(){
    //vec4 texColor = texture(u_Texture, v_TexCord); 
    //color = texColor + u_Color;
    color = u_Color;
}