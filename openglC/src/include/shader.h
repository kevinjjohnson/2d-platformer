#pragma once

uint32_t create_shader(char* filepath, GLenum type);
uint32_t create_shader_program(uint32_t vertexShader, uint32_t fragmentShader);