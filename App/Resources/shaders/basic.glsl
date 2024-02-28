#type vertex

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 Normal;
out vec2 TexCoord;
out vec3 FragPos;

uniform mat4 transform;
uniform mat4 viewProjection;
uniform mat3 normalMatrix;

void main()
{
   gl_Position = viewProjection * transform * vec4(aPos, 1.0);
   Normal = normalMatrix * aNormal;
   TexCoord = aTexCoord;
   FragPos = vec3(transform * vec4(aPos, 1.0));
}

#type fragment

#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;

uniform sampler2D tex;
uniform vec3 viewPos;

struct Material {
    sampler2D texture_diffuse1;
    vec3 diffuse;
    sampler2D texture_specular1;
    vec3 specular;
    float shininess;
}; 

struct DirectionalLight {
    vec3 direction;  
    vec3 diffuse;
    vec3 ambient;
};

struct PointLight{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;

    float radius;
    float intensity;
    float cutOff;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;

    float intensity;
    float cutOff;
    float outerCutOff;
    float linear;
    float quadratic;
}; 
  
uniform Material material;
uniform DirectionalLight directionalLight;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
#define NR_SPOT_LIGHTS 2
uniform SpotLight spotLights[NR_SPOT_LIGHTS];

vec3 calcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir){
    // ambient
    vec3 ambient = light.ambient * (material.diffuse * vec3(texture(material.texture_diffuse1, TexCoord)));
  	
    // diffuse 
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse * vec3(texture(material.texture_diffuse1, TexCoord)));
    
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.diffuse * (spec * material.specular * vec3(texture(material.texture_specular1, TexCoord)));  
        
    return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 frag, vec3 viewDir){
    // ambient
    vec3 ambient  = light.ambient * (material.diffuse * vec3(texture(material.texture_diffuse1, TexCoord)));

    // diffuse 
    float r = light.radius;
    vec3 L = light.position - frag;
    float dist = length(L);
    float d = max(dist - r, 0.0);
    L /= dist;
     
    // calculate basic attenuation
    float denom = d/r + 1.0;
    float attenuation = light.intensity / (denom*denom);
     
    attenuation = (attenuation - light.cutOff) / (1.0 - light.cutOff);
    attenuation = max(attenuation, 0.0);

    float diff = max(dot(normal, L), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse * vec3(texture(material.texture_diffuse1, TexCoord)));
    
    //specular
    vec3 reflectDir = reflect(-L, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.diffuse * (spec * material.specular * vec3(texture(material.texture_specular1, TexCoord)));
    
    return attenuation * (ambient + diffuse + specular);
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 frag, vec3 viewDir){
    vec3 lightDir = normalize(light.position - frag);
    
    // ambient
    vec3 ambient = light.ambient * texture(material.texture_diffuse1, TexCoord).rgb;

    // diffuse 
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * material.diffuse * texture(material.texture_diffuse1, TexCoord).rgb;  
        
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.diffuse * spec * material.specular * texture(material.texture_specular1, TexCoord).rgb;

    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;

    // attenuation
    float distance    = length(light.position - FragPos);
    float attenuation = light.intensity / (1.0 + light.linear * distance + light.quadratic * (distance * distance));    

    ambient  *= attenuation;
    diffuse   *= attenuation;
    specular *= attenuation;   
            
    return ambient + diffuse + specular;
}

void main()
{
    
    vec3 color = vec3(0.0);
    
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    color = calcDirLight(directionalLight, norm, viewDir);
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        color += calcPointLight(pointLights[i], norm, FragPos, viewDir); 

    for(int i = 0; i < NR_SPOT_LIGHTS; i++)
        color += calcSpotLight(spotLights[i], norm, FragPos, viewDir);

    FragColor = vec4(color, 1.0);

}