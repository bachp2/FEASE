#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  

uniform float ambientStrength = 0.1;
uniform vec3 lightPos; 
uniform vec3 lightColor = vec3(1.0);
uniform vec3 objectColor;

void main()
{
    // ambient
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    //if(diff > 0.000) discard;
    vec3 diffuse = diff * lightColor;
     
	// specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(lightPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor; 

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
} 