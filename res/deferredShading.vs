#version 120

varying vec3 normal0;
varying vec4 position0;
varying vec2	texCoord0;

uniform mat4 mvp;
uniform mat4 mmv;

attribute vec2 uv;
attribute vec3 position;
attribute vec3 normal;

void main( void )
{
	gl_Position = mvp * mmv * vec4(position,1.0);
	texCoord0	=  uv;
	normal0 = normalize(( mmv * vec4(normal,0.0)).xyz);
	position0 = mmv * vec4(position,1.0);
}
