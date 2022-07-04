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
