#pragma once
#include "cglm/cglm.h"

typedef struct {
	vec2 position;
	float zoom;

	mat4 projection_matrix;
	mat4 view_matrix;
	mat4 view_projection_matrix;

}camera;

void init_camera(camera* cam);
void calculate_view_projection_matrix(camera* cam);
void move_camera(camera* cam, vec2 translation);
void zoom_camera(camera* cam, float amount);
