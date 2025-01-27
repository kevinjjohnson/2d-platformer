#pragma once
#include "cglm/cglm.h"
#include "quad.h"
#include "batch_renderer.h"
#include "engine.h"

typedef struct {
	quad quad;
	vec2 position;
	vec2 velocity;
	bool is_grounded;
	bool has_jump;
	int movement_dir;
}player;

void init_player(player* player, vec2 location);
void render_player(batch_renderer* renderer, player player);
void move_player(player* player, vec2 amount);
void set_play_pos(player* player, vec2 pos);

void handle_input(frame_input input, player* player);
void handle_horizontal(player* player, float delta_time);
void handle_jump(frame_input input, player* player, float delta_time);
void handle_gravity(frame_input input, player* player, float delta_time);
void apply_physics(player* player, float delta_time);