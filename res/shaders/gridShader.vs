#version 330
#include "PBL_vertex.sl"
#include "basicTransform.sl"

void main()
{
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix  * vec4(position, 1.0);
}