// Uses BasicVertexWithShadows.shader
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
in vec4 v_FragLightPos;

uniform Material u_Material;
uniform Light u_Light;

uniform vec4 u_Color;
uniform vec3 u_lightColor;

uniform vec3 u_camPos;

uniform sampler2D ShadowTex;

uniform sampler2D u_Texture0;
uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;

void main(){
    //vec4 texColor;
    //vec4 ObjectColor;
    //int index  = int(v_TexIndex);

    float distance = length(u_Light.position - v_FragPos);
    float attenuation = 1.0 / (u_Light.constant + u_Light.linear * distance + 
    		    u_Light.quadratic * (distance * distance));    


    vec3 norm = normalize(v_Normalized);
    vec3 lightDir = normalize(u_Light.position - v_FragPos);

    //float theta = dot(lightDir, normalize(-u_Light.lightPoint));


    vec3 viewDir = normalize(u_camPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 hlafwayDir = normalize(lightDir + viewDir);


    vec3 ambient = u_Material.ambient * u_Light.ambient;

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (diff * u_Material.diffuse) * u_Light.diffuse;

    float spec = pow(max(dot(norm, hlafwayDir), 0.0), u_Material.shininess);
    vec3 specular = (spec) * u_Light.specular;


    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    float Shadow = 0.0;
    vec3 lightCords = v_FragLightPos.xyz/ v_FragLightPos.w;
    if(lightCords.z <= 1.0){
        lightCords = (lightCords + 1.0) / 2.0;

        float CloseDepth = texture(ShadowTex, lightCords.xz).r;
        float CurrentDepth = lightCords.z;

        if(CurrentDepth > CloseDepth){
            Shadow = 1.0;
        }
    }

   Finalcolor = vec4((ambient + (diffuse + specular) * (1.0 - Shadow)), 1.0) * u_Color; 
    
}