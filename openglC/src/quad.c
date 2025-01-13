#include "include/quad.h"

void init_quad(quad* q, vec2 size, vec2 location, uint32_t texture) {
	glm_vec2_copy(location, q->location);
	glm_vec2_copy(size, q->size);
	
	float half_width = size[0] / 2;
	float half_height = size[1] / 2;
	//generate points;
	q->corners[0].x = location[0] - half_width;
	q->corners[0].y = location[1] - half_height;
	q->corners[1].x = location[0] + half_width;
	q->corners[1].y = location[1] - half_height;
	q->corners[2].x = location[0] + half_width;
	q->corners[2].y = location[1] + half_height;
	q->corners[3].x = location[0] - half_width;
	q->corners[3].y = location[1] + half_height;
	
	int index;
	//generate edges from the points
	for (index = 0; index < 4; index++) {
		if (index != 3) {
			q->edges[index].x = q->corners[index + 1].x - q->corners[index].x;
			q->edges[index].y = q->corners[index + 1].y - q->corners[index].y;
		}else {
			q->edges[index].x = q->corners[0].x - q->corners[index].x;
			q->edges[index].y = q->corners[0].y - q->corners[index].y;
		}
	}
	q->texture = texture;
}