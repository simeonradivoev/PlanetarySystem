#version 120

varying vec2 texCoord0;

uniform  sampler2D  diffuse;
uniform vec4 color;

void main()
{
	gl_FragColor =  color;
}