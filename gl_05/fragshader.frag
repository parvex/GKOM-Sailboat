#version 330 core
in vec2 TexCoord;
in vec3 vecColor;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;

out vec3 color;

uniform sampler2D Texture;
uniform mat4 MV;
uniform vec3 LightPosition_worldspace;

void main()
{

	vec3 light_direction = vec3(0.603, 0.240, -0.761);
	vec3 light_specular = vec3(0.3, 0.3, 0.3);
	vec3 light_diffuse = vec3(0.8,0.8,0.8);
	vec3 light_ambient = vec3(0.2,0.2,0.2);
	vec3 ambient = light_ambient * vecColor;
  	float material_shininess = 32.0;

    // diffuse 
    vec3 norm = normalize(Normal_cameraspace);
    vec3 lightDir = normalize(light_direction);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light_diffuse * diff * vecColor;  
    
    // specular
    vec3 viewDir = normalize(EyeDirection_cameraspace - Position_worldspace);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material_shininess);
    vec3 specular = light_specular * spec * vecColor;  
        
    vec3 result = ambient + diffuse + specular;
    color = result;
	
}
