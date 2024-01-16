#version 400
out vec4 frag_color;

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;

struct Light {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 viewPos;
uniform Light light;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;


void main()
{
    // Ambient
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
 	
    // Diffuse 
    vec3 norm = normalize(vec3(texture(texture_normal1, TexCoords)) * Normal) ;
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
    
    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));  

    vec3 result = ambient + diffuse + specular;
    frag_color = vec4(result, 1.0f);
} 