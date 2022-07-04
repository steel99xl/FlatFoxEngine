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



#Shader fragment
#version 330 core
struct Light{
    vec3 position;
    vec3 lightPoint;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float Asize;

    float constant;
    float linear;
    float quadratic;

};

struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

layout(location = 0) out vec4 Finalcolor;

in vec3 v_Normalized;
in vec3 v_FragPos;
in vec2 v_TexCord;
in float v_TexIndex;

uniform Material u_Material;
uniform Light u_Light;

uniform vec4 u_Color;
uniform vec3 u_lightColor;

uniform vec3 u_camPos;

uniform sampler2D u_Texture0;
uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;

void main(){
    vec4 texColor;
    vec4 ObjectColor;
    int index  = int(v_TexIndex);

    float distance = length(u_Light.position - v_FragPos);
    float attenuation = 1.0 / (u_Light.constant + u_Light.linear * distance + 
    		    u_Light.quadratic * (distance * distance));    


    vec3 norm = normalize(v_Normalized);
    vec3 lightDir = normalize(u_Light.position - v_FragPos);

    //float theta = dot(u_Light.position, normalize(-u_Light.lightDir));


    vec3 viewDir = normalize(u_camPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    //vec3 hlafwayDir = normalize()

    vec3 ambient = u_Material.ambient * u_Light.ambient;
      
  // do lighting calculations

    float diff = max(dot(norm, u_Light.position), 0.0);
    vec3 diffuse = (diff * u_Material.diffuse) * u_Light.diffuse;

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    vec3 specular = (u_Material.specular * spec) * u_Light.specular;

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

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    Finalcolor = vec4((ambient+ diffuse+ specular), 1.0) * ObjectColor;
    
}