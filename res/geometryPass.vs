#version 420

layout (location = 0) in vec3 position; 
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal; 

out vec3 normal0;
out vec3 position0;
out vec2	texCoord0;

uniform mat4 viewProjectionMatrix;
uniform mat4 modelMatrix;

void main( void )
{
	gl_Position = viewProjectionMatrix * modelMatrix * vec4(position,1.0);
	texCoord0	=  uv;
	normal0 = normalize(( modelMatrix * vec4(normal,0.0)).xyz);
	position0 = (modelMatrix * vec4(position,1.0)).xyz;
}
