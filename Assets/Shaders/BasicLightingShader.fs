#version 450 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 


struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 Normal;  
in vec2 TexCoord;
in vec3 FragPos;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos; 

uniform Material material;
uniform Light light; 

// texture
uniform sampler2D Texture;
 
out vec4 FragColor;

void main()
{
	// Ambient light
    vec3 ambient = light.ambient * material.ambient * lightColor;
	
	// Diffuse light
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos); 
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse *(diff * material.diffuse) * lightColor;
	
	// Specular light
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (material.specular * spec) * lightColor;  
	
	vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0) * texture(Texture, TexCoord);
}