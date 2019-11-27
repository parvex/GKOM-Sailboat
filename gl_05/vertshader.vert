#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 normal;

out vec3 vecColor;
out vec2 TexCoord;
out vec3 Position_worldspace;
out vec3 Normal_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;
  
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 LightPosition_worldspace;

void main()
{
    
    vecColor = color;
    TexCoord = texCoord;




	Position_worldspace = vec3(model * vec4(position, 1.0));
    Normal_cameraspace = mat3(transpose(inverse(model))) * normal;  
    TexCoord = texCoord;
    
    gl_Position = projection * view * vec4(Position_worldspace, 1.0);
} 