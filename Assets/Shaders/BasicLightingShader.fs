#version 450 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 


struct Light {
    vec3 position;
	vec3 direction;	// Directional Lights
  
	// Phong parameters
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
	// Attenuation parameters
	float constant;
    float linear;
    float quadratic;
	
	// Spotlights
	float innerCutOff; 
	float outerCutOff;	
	
	bool isDirectionalLight;
	bool isPointLight;
	bool isSpotLight;
};

in vec3 Normal;  
in vec2 TexCoord;
in vec3 FragPos;
  
uniform vec3 objectColor;
uniform vec3 environmentColor;
uniform vec3 viewPos; 

uniform Material material;
uniform Light light; 

// texture
uniform sampler2D Texture;
 
out vec4 FragColor;



void main()
{
	// Attenuation by distance
	float attenuation = 1.0;    

	if (light.isPointLight || light.isSpotLight)
	{
		float distance    = length(light.position - FragPos);
		attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
	}

	// Ambient light
    vec3 ambient = light.ambient * material.ambient * environmentColor * attenuation;
	
	// Diffuse light
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	if (light.isDirectionalLight)
	{
		lightDir = normalize(-light.direction);
	}	
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse *(diff * material.diffuse) * environmentColor * attenuation;
	
	// Specular light
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (material.specular * spec) * environmentColor * attenuation;  
	
	
	vec3 result = (ambient + diffuse + specular) * objectColor;
    
	if (light.isSpotLight)
	{
		float theta = dot(lightDir, normalize(-light.direction));
		float epsilon   = light.innerCutOff - light.outerCutOff;
		float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0); 
		if (theta < light.innerCutOff)
		{
			result = ambient * objectColor;
		}
		else
		{
			diffuse *= intensity;
			specular *= intensity;
			result = (ambient + diffuse + specular) * objectColor;
		}
	}
	
	FragColor = vec4(result, 1.0) * texture(Texture, TexCoord);
}