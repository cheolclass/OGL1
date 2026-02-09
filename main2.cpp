#include <glad/glad.h>  // 이걸 먼저 선언해야함
#include <GLFW/glfw3.h>
#include <iostream>

// 창 크기가 변경될 때마다 호출되는 콜백 함수
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

// 사용자 입력 처리 (ESC 누르면 창 닫기)
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// 셰이더 소스 코드 (보통은 외부 파일에서 읽지만, 여기서는 문자열로 선언합니다)
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n" // 주황색
"}\n\0";

int main() 
{	
	// 1. GLFW 초기화 및 설정
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 2. 윈도우 생성
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL - Triangle", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// 3. GLAD 초기화
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 4. 셰이더 컴파일 및 링크 
	// Vertex Shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); 
	glCompileShader(vertexShader);  // 쉐이더 코드를 GPU가 이해할 수 있게 컴파일

	// Fragment Shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Shader Program 링크
	unsigned int shaderProgram = glCreateProgram();  // 쉐이더 기능을 넣을 프로그램 객체 생성
	glAttachShader(shaderProgram, vertexShader);  // 쉐이더 2개를 프로그램에 연결(넣기)
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);  // 프로그램 내 셰이더들을 서로 연결 + 하나의 실행 가능한 파이프라인으로 만들기 

	// 컴파일 후 셰이더 객체 삭제 (프로그램에 링크되었으므로 더 이상 필요 없음)
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// 5. 데이터 설정 (VBO, VAO)
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // 왼쪽 아래
		 0.5f, -0.5f, 0.0f, // 오른쪽 아래
		 0.0f,  0.5f, 0.0f  // 위쪽 중앙
	};

	unsigned int VBO, VAO;  // 버텍스 버퍼 객체, 버텍스 배열 객체
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 데이터 해석 방법 정의
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 바인딩 해제
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// 6. 렌더링 루프
	while (!glfwWindowShouldClose(window)) {
		// 입력 확인
		processInput(window);

		// 화면 지우기 (배경색 설정)
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// 삼각형 그리기
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// 버퍼 교체 및 이벤트 조사
		glfwSwapBuffers(window);
		glfwPollEvents();  // 사용자의 입력을 기다림
	}

	// 7. 자원 해제
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}