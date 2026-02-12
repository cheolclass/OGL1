#ifndef SHADER_HPP
#define SHADER_HPP

GLuint compileShader(GLenum type, const std::string& path);
GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);

#endif // SHADER_HPP