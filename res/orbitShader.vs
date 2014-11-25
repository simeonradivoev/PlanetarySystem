#version 120

attribute vec3 position;

uniform  mat4 mvp;
uniform  mat4 mmv;

void main()
{
	gl_Position = mvp* mmv  * vec4(position, 1.0);
}