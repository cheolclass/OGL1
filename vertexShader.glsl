#version 330 core 
layout (location = 0) in vec3 aPos; 
void main() 
{ 
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	//gl_Position.xyz = vertexPosition_modelspace;
	//gl_Position.w = 1.0;
}
