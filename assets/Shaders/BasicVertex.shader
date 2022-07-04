#Shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normalizedpos;
layout(location = 2) in vec2 texCord;
layout(location = 3) in float texIndex;

out vec3 v_Normalized;
out vec3 v_FragPos;
out vec2 v_TexCord;
out float v_TexIndex;

uniform mat4 Modle;
uniform mat4 View;
uniform mat4 Projection;

void main(){
gl_Position = Projection * View * Modle * vec4(position, 1.0);
v_FragPos = vec3(Modle * vec4(position, 1.0));
v_Normalized = normalizedpos;
v_TexCord  = texCord;
v_TexIndex = texIndex;
}
