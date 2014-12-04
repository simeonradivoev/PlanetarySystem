#version 420
#include "basicTransform.sl"
#include "PBL_vertex.sl"

void main( void )
{
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position,1.0);
	texCoord0	=  uv;
	normal0 = normalize(( ModelMatrix * vec4(normal,0.0)).xyz);
	position0 = (ModelMatrix * vec4(position,1.0)).xyz;
}
