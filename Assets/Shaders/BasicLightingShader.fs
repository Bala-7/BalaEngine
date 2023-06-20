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
in float interpolatedDepth;
in vec4 ShadowCoord;  
  
uniform vec3 objectColor;
uniform vec3 environmentColor;
uniform vec3 viewPos; 
uniform sampler2D shadowMap;

uniform Material material;
uniform Light light; 

// texture
uniform sampler2D Texture;
 
out vec4 FragColor;

float ShadowCalculation(vec4 fragPosLightSpace)
{
	// perform perspective divide
	vec3 projCoords = (fragPosLightSpace.xyz / fragPosLightSpace.w) * 0.5 + 0.5;
	
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	
	float currentDepth = projCoords.z;
	float shadow = currentDepth > closestDepth ? 1.0 : 0.0;
	
	return shadow;
}

void main()
{
	// This is required by the editor texture to render objects with proper depth 
	float fragmentDepth = gl_FragCoord.z / gl_FragCoord.w;
	if (fragmentDepth < interpolatedDepth) {
        // Fragment is behind, discard or do additional processing
        discard;
    }

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
	
	
	// Shadows
	float shadow = ShadowCalculation(ShadowCoord);
	shadow = 0;
	vec3 result = (ambient + (1.0 - shadow)*(diffuse + specular)) * objectColor;
    
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