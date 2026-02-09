#include <glad/glad.h>  /// glfw3.h보다 glad.h를 먼저 포함해야 함 
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
	// 1. GLFW 초기화 및 설정
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // OpenGL 3.x 버전 사용. 4.x도 가능
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  // OpenGL x.3 버전 사용. 5.x도 가능
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.3 코어 프로파일 사용. 즉, 이전 버전의 오래된 기능들은 사용 안함
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);  // 레거시 기능 사용함 
	// glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);  // 창 크기 변경 불가

	// 2. 윈도우 생성
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
		// 네번째 인자 -> glfwGetPrimaryMonitor() -> 전체 화면 모드

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);  // 이 창에서 사용되는 모든 OpenGL 명령어가 '이 window'의 캔버스에 그려지도록 연결 
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  // 창 크기 변경 콜백 함수 등록

	// 3. GLAD 초기화 (모든 OpenGL 함수 주소 로드).  가져온 후 내부적으로 모두 각각 저장
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 4. 이벤트 처리 무한루프
	while (!glfwWindowShouldClose(window))  
	{
		// 이벤트 처리 콜백 함수 호출
		processInput(window);  

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// 5. 종료: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE); // true
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
