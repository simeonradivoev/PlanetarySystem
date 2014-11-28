#version 120

varying vec3 normal0;
varying vec4 position0;
varying vec2	texCoord0;

uniform mat4 viewProjectionMatrix;
uniform mat4 modelMatrix;

attribute vec2 uv;
attribute vec3 position;
attribute vec3 normal;

void main( void )
{
	gl_Position = viewProjectionMatrix * modelMatrix * vec4(position,1.0);
	texCoord0	=  uv;
	normal0 = normalize(( modelMatrix * vec4(normal,0.0)).xyz);
	position0 = viewProjectionMatrix * modelMatrix * vec4(position,1.0);
}
