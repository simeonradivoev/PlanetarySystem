#version 120

attribute vec3 position;
attribute vec2 uv;

varying vec2 texCoord0;

uniform  mat4 mvp;
uniform  mat4 mmv;

void main()
{
	gl_Position = mvp* mmv  * vec4(position, 1.0);
	texCoord0 = uv;
}