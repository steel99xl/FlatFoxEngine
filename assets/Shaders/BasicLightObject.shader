// Uses BasicVertex.shader
#Shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_Normalized;
in vec3 v_FragPos;
in vec2 v_TexCord;
in float v_TexIndex;

uniform vec4 u_Color;

uniform sampler2D u_Texture0;
uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;

void main(){
    vec4 texColor;
    vec4 ObjectColor;
    int index  = int(v_TexIndex);

    if(index == 1){
        texColor = texture(u_Texture0, v_TexCord); 
    } else if(index == 2){
        texColor = texture(u_Texture1, v_TexCord); 
    } else if(index == 3){
        texColor = texture(u_Texture2, v_TexCord); 
    } else {
        texColor = u_Color;
    }
    if(texColor == u_Color){
        ObjectColor = texColor;
    } else{
        ObjectColor = texColor + u_Color;
    }

    color = ObjectColor;
}