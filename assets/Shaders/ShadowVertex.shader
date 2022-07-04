#Shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normalizedpos;
layout(location = 2) in vec2 texCord;
layout(location = 3) in float texIndex;

out vec3 v_FragPos;

uniform mat4 SModle;
uniform mat4 SView;
uniform mat4 SProjection;

void main(){
    gl_Position = SProjection * SView * SModle * vec4(position, 1.0);
    v_FragPos = vec3(SModle * vec4(position, 1.0));
}