#pragma once
#include "quad.h"

typedef struct {
	int level_id;
	int num_spawns;
	vec2* spawns;
	quad* drawables;
	int num_drawables;

}level;

void save_level(char* filepath, level l);
void load_level(char* filepath);

void write_quad(char* filepath, quad q);
void read_quad(char* filepath);