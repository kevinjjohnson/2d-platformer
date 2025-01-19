#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "include/shader.h"

uint32_t create_shader(char* filepath, GLenum type) {
	long size;
	char source[1000];
	FILE* file;
	uint32_t shader;
	int success;

	fopen_s(&file, filepath, "rb");
	if (file == NULL) {
		printf("ERROR OPENING SHADER FILE");
		return -1;
	}
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, 0, 0);
	fread((void*)source, 1, size, file);
	source[size] = '\0';
	//printf(source);
	//printf("\nfile size is: %ld bytes", size);
	shader = glCreateShader(type);
	char infoLog[512];
	const char* src = source;
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		if(type == GL_VERTEX_SHADER)
			printf("ERROR IN VERTEX SHADER COMPILATION: \n");
		else
			printf("ERROR IN FRAGMENT SHADER COMPILATION: \n");
		printf(infoLog);
		printf("\n");
		return 0;
	}
	return shader;
}

uint32_t create_shader_program(uint32_t vertexShader, uint32_t fragmentShader)
{
	uint32_t program;
	int success;
	char infoLog[500];

	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		printf("ERROR IN SHADER LINKING: ");
		printf(infoLog);
		printf("\n");
		return 0;
	}
	glUseProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return program;
}
