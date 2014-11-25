#version 120

attribute vec3 position;
attribute vec2 uv;
attribute vec3 normal;

varying vec2 texCoord0;
varying vec3 normal0;

uniform  mat4 mvp;
uniform  mat4 mmv;


void main()
{
	gl_Position = mvp* mmv * vec4(position, 1.0);
	texCoord0 = uv;
	//just rotate normals
	normal0 =  (mvp * mmv * vec4(normal,0.0)).xyz;
}