#pragma once
#include <stdint.h>
#include <stdio.h>
#include "cglm/cglm.h"
#include "quad.h"

typedef struct {
	float x;
	float y;
}vec_2;

typedef struct {
	vec_2 pos;
	vec_2 texture_coord;
	float texture;
}vertex;

typedef struct {
	uint32_t num_quads;

	uint32_t va;
	uint32_t vb;
	uint32_t ib;

	vertex* buffer_start;
	vertex* buffer_current;
	int num_draw_calls;
}batch_renderer;

void init_batch_renderer(batch_renderer* renderer);
void free_batch_renderer(batch_renderer* renderer);
void draw_batch(batch_renderer* renderer);
void add_quad(batch_renderer* renderer, quad q);
void flush_renderer(batch_renderer* renderer);
