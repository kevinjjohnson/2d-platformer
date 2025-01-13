#include "include/batch_renderer.h"
#include <GL/glew.h>
#include <stdlib.h>
#include <string.h>

#define MAX_QUAD_COUNT  1000
#define MAX_VERTEX_COUNT MAX_QUAD_COUNT * 4
#define MAX_INDEX_COUNT MAX_QUAD_COUNT * 6



uint32_t testindicies[] = {
	0, 1, 2, 2, 3, 0,
	4, 5, 6, 6, 7, 4,
	8, 9, 10, 10, 11, 8
};

float testverts[] = {
	 150.0f,  150.0f,  1.0f, 1.0f, 1.0f,
	 150.0f,  50.0f,  1.0f, 0.0f, 1.0f,
	 50.0f,  50.0f,  0.0f, 0.0f, 1.0f,
	 50.0f,  150.0f,  0.0f, 1.0f, 1.0f,
	 
	 850.0f,  500.0f,  1.0f, 1.0f, 1.0f,
	 850.0f,  400.0f,  1.0f, 0.0f, 1.0f,
	 750.0f,  400.0f,  0.0f, 0.0f, 1.0f, 
	 750.0f,  500.0f,  0.0f, 1.0f, 1.0f,
};

void init_batch_renderer(batch_renderer* renderer) {
	printf("test");
	renderer->num_quads = 0;
	renderer->num_draw_calls = 0;

	glGenVertexArrays(1, &(renderer->va));
	glBindVertexArray(renderer->va);

	glGenBuffers(1, &(renderer->vb));
	glBindBuffer(GL_ARRAY_BUFFER, renderer->vb);
	glBufferData(GL_ARRAY_BUFFER, MAX_VERTEX_COUNT * sizeof(vertex), ((void*)0), GL_DYNAMIC_DRAW);

	//generate index buffer
	uint32_t indices[MAX_QUAD_COUNT * 6];
	uint32_t i, offset;
	offset = 0;
	for (i = 0; i < MAX_QUAD_COUNT * 6; i += 6) {
		indices[i] = offset + 0;
		indices[i + 1] = offset + 1;
		indices[i + 2] = offset + 2;
		indices[i + 3] = offset + 2;
		indices[i + 4] = offset + 3;
		indices[i + 5] = offset + 0;
		offset += 4;
	}

	glGenBuffers(1, &(renderer->ib));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(4 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	renderer->buffer_start = malloc(MAX_VERTEX_COUNT * sizeof(vertex));
	renderer->buffer_current = renderer->buffer_start;

}

void free_batch_renderer(batch_renderer* renderer){
	free(renderer->buffer_start);
}

void draw_batch(batch_renderer* renderer){
	glBindBuffer(GL_ARRAY_BUFFER, renderer->vb);
	glBufferSubData(GL_ARRAY_BUFFER, 0, renderer->num_quads * 4 * sizeof(vertex), renderer->buffer_start);

	glBindVertexArray(renderer->va);
	glDrawElements(GL_TRIANGLES, renderer->num_quads * 6, GL_UNSIGNED_INT, 0);
	renderer->num_draw_calls++;
}

void add_quad(batch_renderer* renderer, quad q) {
	if (renderer->num_quads >= MAX_QUAD_COUNT) {
		draw_batch(renderer);
		flush_renderer(renderer);
	}
	/*
	* 3--2
	* |  |
	* 0--1
	*/
	vertex verts[4];
	int i;
	for (i = 0; i < 4; i++) {
		verts[i].pos.x = q.corners[i].x;
		verts[i].pos.y = q.corners[i].y;
		verts[i].texture = q.texture;
	}
	verts[0].texture_coord.x = 0.0f;
	verts[0].texture_coord.y = 0.0f;
	verts[1].texture_coord.x = 1.0f;
	verts[1].texture_coord.y = 0.0f;
	verts[2].texture_coord.x = 1.0f;
	verts[2].texture_coord.y = 1.0f;
	verts[3].texture_coord.x = 0.0f;
	verts[3].texture_coord.y = 1.0f;

	memcpy(renderer->buffer_current, verts, sizeof(verts));
	renderer->buffer_current += 4;
	renderer->num_quads++;
}

void flush_renderer(batch_renderer* renderer){
	renderer->num_quads = 0;
	renderer->buffer_current = renderer->buffer_start;
}
