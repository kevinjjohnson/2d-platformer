#pragma once
#include "cglm/cglm.h"
#include "collison.h"

typedef struct {
	vec2 size;
	vec2 location;
	point corners[4];
	edge edges[4];
	uint32_t texture;
} quad;

void init_quad(quad* q, vec2 size, vec2 location, uint32_t texture);
void quad_set_pos(quad* q, vec2 pos);
void quad_set_size(quad* q, vec2 size);