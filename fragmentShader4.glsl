#version 330 core 
out vec4 color; 
in	vec3 ourColor;  // 버텍스 셰이더에서 받은 색상

void main() 
{ 
   //color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
   color = vec4(ourColor, 1.0f);
}

