#version 330 core

layout (location = 0) in vec3 aPos;

out vec4 FragPos;

uniform mat4 worldPos;
uniform mat4 projectionMatrix;

void main()
{
  FragPos = worldPos * vec4(aPos, 1.0);
  gl_Position = projectionMatrix * worldPos * vec4(aPos.x, aPos.y, aPos.z, 1.0);  
}	    
