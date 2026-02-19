#version 330 core 
layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 aColor; 

out vec3 ourColor;  // 프래그먼트 셰이더로 전달할 색상

void main() 
{ 
	gl_Position = vec4(aPos, 1.0);
	//gl_Position.xyz = vertexPosition_modelspace;
	//gl_Position.w = 1.0;

	ourColor = aColor; // 입력받은 색상을 그대로 출력
}
