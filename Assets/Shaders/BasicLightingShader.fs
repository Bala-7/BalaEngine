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

#define MAX_LIGHTS_COUNT 10
uniform Light sceneLights[MAX_LIGHTS_COUNT];
uniform int activeLights;

in vec3 Normal;  
in vec2 TexCoord;
in vec4 DecalCoords;
in vec3 FragPos;
in vec4 FragPosLightSpace;  
in float interpolatedDepth;

uniform vec3 lightPos;
uniform float far_plane;
  
uniform bool enableDecal;  
uniform vec3 objectColor;
uniform vec3 environmentColor;
uniform vec3 viewPos; 
uniform sampler2D directionalLightShadowMap;
uniform samplerCube pointLightShadowCubeMap;

uniform int debugMode;

uniform Material material;

uniform float pointLightsFarPlane;
in vec4 FragPosCubeMap;

// texture
uniform sampler2D Texture;
uniform sampler2D decalTexture;

 
out vec4 FragColor;

// Returns shadow: 1.0f if the object is in light, else 0.0f
float PointLightShadowCalculation()
{
	// Get vector between fragment position and light position
	vec3 fragToLight = FragPos - lightPos;
	
	// Use the light to fragment vector to sample from the depth map
	float closestDepth = texture(pointLightShadowCubeMap, fragToLight).r;
	
	// It is currently in linear range between [0,1]. Re-transform back to original value
	closestDepth *= far_plane;
	
	// Now get current linear depth as the length between the fragment and light position
	float currentDepth = length(fragToLight);

	// Now test for shadows
	float bias = 0.05f;
	float shadow = currentDepth - bias > closestDepth ? 0.0 : 1.0;
	return shadow;
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
	// perform perspective divide
	vec3 projCoords = (fragPosLightSpace.xyz / fragPosLightSpace.w) * 0.5 + 0.5;

	float closestDepth = texture(directionalLightShadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	
	float bias = 0.005;
	//float bias = max(0.005 * (1.0 - dot(Normal, light.direction)), 0.005); // Not working
	float shadow = currentDepth-bias > closestDepth ? 0.0 : 1.0;
	
	if(fragPosLightSpace == vec4(0.0, 0.0, 0.0, 0.0))
	{
		return 1;
	}
	
	if(projCoords.z > 1.0)
		shadow = 1.0;
	
	return shadow;
}

vec3 LightCalculation(Light light, float shadow, float pointShadow)
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
	
	//if(pointShadow != 0)
	//{
		//ambient = light.ambient * vec3(1.0, 0.0, 0.0) * environmentColor * attenuation; 
	//}
	
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
	
	if (light.isSpotLight)
	{
		float theta = dot(lightDir, normalize(-light.direction));
		float epsilon   = light.innerCutOff - light.outerCutOff;
		float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0); 
		if (theta < light.innerCutOff)
		{
			return ambient;
		}
		else
		{
			diffuse *= intensity;
			specular *= intensity;
		}
	}
	
	
	float finalShadow = 1.0f;
	//if(shadow == 0.0f && pointShadow == 0.0f)
		//finalShadow = 0.0f;
	
	finalShadow = pointShadow;
	
	vec3 result = (ambient + (finalShadow) * (diffuse + specular));
	return result;
}

void main()
{
	// This is required by the editor texture to render objects with proper depth 
	float fragmentDepth = gl_FragCoord.z / gl_FragCoord.w;
	if (fragmentDepth < interpolatedDepth) {
        // Fragment is behind, discard or do additional processing
        discard;
    }
	
	
	// Shadows
	float shadow = ShadowCalculation(FragPosLightSpace);
	
	float pointLightShadow = PointLightShadowCalculation();
	
	// Light calculations
	vec3 result = LightCalculation(sceneLights[0], shadow, pointLightShadow);
	for(int i = 1; i < activeLights; ++i)
		result += LightCalculation(sceneLights[i], shadow, pointLightShadow);
	
	
	result *= objectColor;
    
	vec4 finalColor = vec4(result, 1.0) * texture(Texture, TexCoord);
	
	// Decal
	if(enableDecal)
	{
		vec3 projCoords = DecalCoords.xyz / DecalCoords.w;
		projCoords = projCoords * 0.5 + 0.5; // Transform to [0, 1] range

		// Check if the projected coordinates are within the texture bounds
		if (projCoords.x >= 0.0 && projCoords.x <= 1.0 &&
			projCoords.y >= 0.0 && projCoords.y <= 1.0 &&
			projCoords.z >= 0.0 && projCoords.z <= 1.0)
		{
			vec4 decalColor = texture(decalTexture, projCoords.xy);
			FragColor = mix(finalColor, decalColor, decalColor.a);
		}
		else
		{
			FragColor = finalColor;
		}
	}
	else
	{
		FragColor = finalColor;
	}	
}