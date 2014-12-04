#version 420
#include "basicTransform.sl"

layout (location = 0) in vec3 position; 
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal; 

out vec3 normal0;
out vec3 position0;
out vec2	texCoord0;

void main( void )
{
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position,1.0);
	texCoord0	=  uv;
	normal0 = normalize(( ModelMatrix * vec4(normal,0.0)).xyz);
	position0 = (ModelMatrix * vec4(position,1.0)).xyz;
}
