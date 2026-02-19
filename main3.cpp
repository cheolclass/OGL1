#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.hpp"

int main()
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);  // MSAA(Multi-Sample Anti-Aliasing) 설정을 활성화
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);	
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // 구버전 호환X. macOS 필수!

	GLFWwindow* window = glfwCreateWindow(800, 600, "My Window", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// 모든 OpenGL 함수 포인터를 로드 (연결) 
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 쉐이더 객체 생성부터 컴파일, 링크, 소멸까지 원스톱으로 처리
	GLuint shaderID = LoadShaders("vertexShader.glsl", "fragmentShader.glsl");

	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,
	};
	
	GLuint VAO;	
	glGenVertexArrays(1, &VAO); 	
	glBindVertexArray(VAO);
	/// 쉐이더 생성

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);  // 데이터를 현재 바인딩된 버퍼에 물리적 복사. CPU 정점 배열 데이터 -> GPU 버퍼 메모리로 통째로 복사
	gladLoadGL();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);  // VBO에 복사된 데이터를 어떻게 읽어야 하는지(접근 밥법)를 VAO에 알려줌(연결)
	glEnableVertexAttribArray(0);  // 0번에 위치한 정점 속성 사용을 활성화. 비로소 GPU가 렌더링 시 해당 데이터를 참조 가능하게 됨
	glBindVertexArray(0);  // VAO 해제. VBO보다 먼저 해제해야 함. VAO가 VBO의 상태를 기억하기 때문
	glBindBuffer(GL_ARRAY_BUFFER, 0);  // VBO 해제

	glUseProgram(shaderID);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f); 

	do
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3); // 인덱스 버퍼 없이 정점 데이터를 순차적으로 읽어와 그림
				// glDrawElements() -> 인덱스 버퍼를 통해 정점 데이터 참조 -> 중복X, 메모리 효율성 향상 
				// 다음 예제에	서 다룸 

		glfwSwapBuffers(window);
		glfwPollEvents();

	} while (glfwWindowShouldClose(window) == 0 && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderID);

	glfwTerminate();

	return 0;
}
