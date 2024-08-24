#version 450 core

in vec3 Normal;  
in vec2 TexCoord;
in vec3 FragPos;
in float interpolatedDepth;

uniform vec3 pickingColor;

 
out vec4 FragColor;



void main()
{
	// This is required by the editor texture to render objects with proper depth 
	//float fragmentDepth = gl_FragCoord.z / gl_FragCoord.w;
	//if (fragmentDepth < interpolatedDepth) {
        // Fragment is behind, discard or do additional processing
    //    discard;
    //}
	
	FragColor = vec4(pickingColor, 1.0);	
}