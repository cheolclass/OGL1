#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <glad/glad.h>  // 반드시 다른 GL 관련 헤더보다 먼저
#include "shader.hpp"

// 파일 읽기 및 컴파일 체크 전용 보조 함수
GLuint compileShader(GLenum type, const std::string& path) {
	std::ifstream stream(path);
	if (!stream.is_open()) {
		std::cerr << "Failed to open: " << path << std::endl;
		return 0;
	}

	std::stringstream sstr;
	sstr << stream.rdbuf();
	std::string code = sstr.str();
	const char* src = code.c_str();

	GLuint id = glCreateShader(type);
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	// 에러 체크
	GLint result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		std::vector<char> error(length);
		glGetShaderInfoLog(id, length, nullptr, error.data());
		std::cerr << "Shader Error (" << path << "): " << error.data() << std::endl;
		glDeleteShader(id);
		return 0;
	}
	return id;
}

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {
	GLuint vShader = compileShader(GL_VERTEX_SHADER, vertex_file_path);
	GLuint fShader = compileShader(GL_FRAGMENT_SHADER, fragment_file_path);

	if (!vShader || !fShader) return 0;

	GLuint programID = glCreateProgram();
	glAttachShader(programID, vShader);
	glAttachShader(programID, fShader);
	glLinkProgram(programID);

	// 링크 에러 체크
	GLint result;
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &length);
		std::vector<char> error(length);
		glGetProgramInfoLog(programID, length, nullptr, error.data());
		std::cerr << "Link Error: " << error.data() << std::endl;
	}

	// 정리: 링크 후에는 개별 쉐이더 객체가 필요 없음
	glDeleteShader(vShader);
	glDeleteShader(fShader);

	return programID;
}