#include "include/character.h"

static float ground_deceleration = 4000.0f;
static float max_velocity = 200.0f;
static float acceleration = 4000.0f;
static float jump_force = 300.0f;
static float max_fall_speed = 500;
static float gravity = -500.0f;

void init_player(player* player, vec2 location) {
	player->is_grounded = false;
	player->position[0] = location[0];
	player->position[1] = location[1];
	init_quad(&player->quad, (vec2) { 20, 40 }, player->position, 0);
}

void render_player(batch_renderer* renderer, player player) {
	add_quad(renderer, player.quad);
}

void move_player(player* player, vec2 amount) {
	player->position[0] += amount[0];
	player->position[1] += amount[1];

	quad_set_pos(&player->quad, player->position);
}

void set_play_pos(player* player, vec2 pos) {
	player->position[0] = pos[0];
	player->position[1] = pos[1];

	quad_set_pos(&player->quad, player->position);
}

void handle_horizontal(player* player, float delta_time) {
	float new_velocity;
	if (player->movement_dir == 0) {
		//printf("stopped/decelarating\n");
		if (player->velocity[0] < 0) {
			new_velocity = player->velocity[0] + ground_deceleration * delta_time;
			player->velocity[0] = fmin(new_velocity, 0.0f);
		}
		else if (player->velocity[0] > 0) {
			new_velocity = player->velocity[0] +  -1 * ground_deceleration * delta_time;
			player->velocity[0] = fmax(new_velocity, 0.0f);
		}
	}
	else if (player->movement_dir == 1) {
		//printf("moving right\n");
		if (player->velocity[0] < max_velocity) {
			new_velocity = player->velocity[0] + acceleration * delta_time;
			player->velocity[0] = min(new_velocity, max_velocity);
		}
	}
	else if (player->movement_dir == -1) {
		//printf("moving left\n");
		if (player->velocity[0] > -1 * max_velocity) {
			new_velocity = player->velocity[0] + -1 * acceleration * delta_time;
			player->velocity[0] = max(new_velocity, -1 * max_velocity);
		}
	}

}

void handle_jump(player* player, float delta_time) {
	if (player->has_jump && player->is_grounded) {
		player->velocity[1] = jump_force;
		player->is_grounded = false;
		player->has_jump = false;
	}
}

void handle_gravity(player* player, float delta_time) {
	if (player->is_grounded) {
		player->velocity[1] = -0.0f;
	}
	else {
		player->velocity[1] += delta_time * gravity;
	}
}

void apply_physics(player* player, float delta_time) {
	player->position[0] = player->position[0] + delta_time * player->velocity[0];
	player->position[1] = player->position[1] + delta_time * player->velocity[1];
	quad_set_pos(&player->quad, player->position);
}
