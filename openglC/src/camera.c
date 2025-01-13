#include "include/camera.h"

void init_camera(camera* cam) {
	cam->position[0] = 0;
	cam->position[1] = 0;
	cam->zoom = 1.0f;
	glm_ortho(0.0f, 1600.0f, 0.0f, 900.0f, -1.0f, 1.0f, cam->projection_matrix);

	glm_mat4_identity(cam->view_matrix);
	glm_translate(cam->view_matrix, (vec3) { 500.0f, 200.0f, 0.0f });
	glm_mat4_mul(cam->projection_matrix, cam->view_matrix, cam->view_projection_matrix);
}

void calculate_view_projection_matrix(camera* cam) {
	glm_mat4_identity(cam->view_matrix);
	glm_translate(cam->view_matrix, (vec3) { -1.0 * cam->position[0], -1.0 * cam->position[1], 0.0f });
	glm_mat4_mul(cam->projection_matrix, cam->view_matrix, cam->view_projection_matrix);
}

void move_camera(camera* cam, vec2 translation) {
	cam->position[0] += translation[0];
	cam->position[1] += translation[1];
}

void zoom_camera(camera* cam, float amount) {
	cam->zoom = amount;
	if (cam->zoom <= 0) cam->zoom = .01;

	glm_ortho(0.0f, 1600.0f * cam->zoom, 0.0f, 900.0f * cam->zoom, -1.0f, 1.0f, cam->projection_matrix);
}


