#Shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCord;

out vec2 v_TexCord;

uniform mat4 u_MVP;

void main(){
gl_Position = u_MVP * position;
v_TexCord  = texCord;
}



#Shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;
void main(){
vec4 texColor = texture(u_Texture, v_TexCord); 
color = texColor;
}
