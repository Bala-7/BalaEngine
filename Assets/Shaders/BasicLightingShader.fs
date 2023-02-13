#version 450 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 


in vec3 Normal;  
in vec2 TexCoord;
in vec3 FragPos;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos; 
uniform Material material;

// texture
uniform sampler2D Texture;
 
out vec4 FragColor;

void main()
{
	// Ambient light
    vec3 ambient = vec3(0.2f) * material.ambient * lightColor;
	
	// Diffuse light
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos); 
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = vec3(0.5f) *(diff * material.diffuse) * lightColor;
	
	// Specular light
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = vec3(1.0f) * (material.specular * spec) * lightColor;  
	
	vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0) * texture(Texture, TexCoord);
}