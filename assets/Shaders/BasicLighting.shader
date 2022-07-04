// Uses BasicVertes.shader
#Shader fragment
#version 330 core
#define MAX_POINT_LIGHT 4 //you probaly wont need more than that per object, lol
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
uniform Light lights[MAX_POINT_LIGHT];

uniform vec4 u_Color;
uniform vec3 u_lightColor;

uniform vec3 u_camPos;

uniform sampler2D u_Texture0;
uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;

vec3 PointLight(Light light){
    vec3 Output;

    float distance = length(light.position - v_FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
    		    light.quadratic * (distance * distance));    


    vec3 norm = normalize(v_Normalized);
    vec3 lightDir = normalize(light.position - v_FragPos);

    //float theta = dot(lightDir, normalize(-u_Light.lightPoint));


    vec3 viewDir = normalize(u_camPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 hlafwayDir = normalize(lightDir + viewDir);


    vec3 ambient = u_Material.ambient * light.ambient;

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (diff * u_Material.diffuse) * light.diffuse;

    float spec = pow(max(dot(norm, hlafwayDir), 0.0), u_Material.shininess);
    vec3 specular = (spec) * light.specular;


    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    Output = (ambient + diffuse + specular);

    return Output;
}


void main(){
    //vec4 texColor;
    //vec4 ObjectColor;
    //int index  = int(v_TexIndex);

    vec3 LightOutput = vec3(0.0);
    vec3 TempLight = vec3(0.0);

    for(int i = 0; i < MAX_POINT_LIGHT; i++){
        TempLight = PointLight(lights[i]);

        LightOutput = LightOutput + TempLight;
    }

    Finalcolor = vec4(LightOutput, 1.0) * u_Color;
    

        
    //if(theta > u_Light.Asize) {          
  // do lighting calculations
   //    Finalcolor = vec4((ambient+ diffuse+ specular), 1.0) * ObjectColor; 
 //   }else{

//        Finalcolor = vec4(ambient,1.0f) * ObjectColor;

//    }  // else, use ambient light so scene isn't completely

   //Finalcolor = vec4((ambient+ diffuse+ specular), 1.0) * u_Color; 
    
}