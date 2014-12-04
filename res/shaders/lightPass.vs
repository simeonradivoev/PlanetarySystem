#version 330 
#include "basicTransform.sl"

layout (location = 0) in vec3 position; 

void main()
{ 
   	gl_Position = ProjectionMatrix * ViewMatrix  * ModelMatrix* vec4(position, 1.0);
} 