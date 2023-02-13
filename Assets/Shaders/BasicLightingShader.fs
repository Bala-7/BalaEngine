#version 450 core

in vec3 Normal;  
in vec2 TexCoord;
in vec3 FragPos;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos; 

// texture
uniform sampler2D Texture;
 
out vec4 FragColor;

void main()
{
	// Ambient light
	float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
	
	// Diffuse light
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos); 
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	
	vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0) * texture(Texture, TexCoord);
}