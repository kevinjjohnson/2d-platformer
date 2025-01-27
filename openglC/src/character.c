#include "include/character.h"

static float ground_deceleration = 4000.0f;
static float max_velocity = 200.0f;
static float acceleration = 4000.0f;
static float jump_force = 400.0f;
static float max_fall_speed = -1000;
static float gravity = -600.0f;
static int dir = 0;
static bool ended_early;
static float ended_early_modifier = 3.0f;
static bool input_available = true;

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

void handle_input(frame_input input, player* player) {
	printf("input available: %s\n", input_available ? "true" : "false");
	printf("space held: %s\n", input.space_held ? "true" : "false");
	if (!input.a_held && !input.d_held)
		dir = 0;
	else if (input.a_held && input.d_held && input.last != dir) {
		dir = input.last;
	}
	else if (input.a_held && !input.d_held) {
		dir = -1;
	}
	else if (input.d_held && !input.a_held) {
		dir = 1;
	}
	if ((!input.space_held) && (!input_available) && player->is_grounded) {
		input_available = true;
	}
	//printf("Direction is: %d\n", dir);
}

void handle_horizontal(player* player, float delta_time) {
	float new_velocity;
	if (dir == 0) {
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
	else if (dir == 1) {
		//printf("moving right\n");
		if (player->velocity[0] < max_velocity) {
			new_velocity = player->velocity[0] + acceleration * delta_time;
			player->velocity[0] = min(new_velocity, max_velocity);
		}
	}
	else if (dir == -1) {
		//printf("moving left\n");
		if (player->velocity[0] > -1 * max_velocity) {
			new_velocity = player->velocity[0] + -1 * acceleration * delta_time;
			player->velocity[0] = max(new_velocity, -1 * max_velocity);
		}
	}

}

void handle_jump(frame_input input, player* player, float delta_time) {
	printf("player is grounded: %s\n", player->is_grounded ? "true" : "false");
	if (input.space_held && player->is_grounded && input_available) {
		player->has_jump = true;
		printf("JUMPING\n");
		input_available = false;
	}
		
	if (!player->is_grounded && !ended_early && !input.space_held && player->velocity[1] > 0) {
		ended_early = true;
		printf("ended early\n");
	}
		

	if (player->has_jump && player->is_grounded) {
		player->velocity[1] = jump_force;
		player->is_grounded = false;
		player->has_jump = false;
		ended_early = false;
	}
}

void handle_gravity(frame_input input, player* player, float delta_time) {
	if (player->is_grounded) {
		player->velocity[1] = -5.0f;
	}
	else {
		float downward_vel = gravity;
		if (ended_early) downward_vel *= ended_early_modifier;
		player->velocity[1] = fmax(player->velocity[1] + delta_time * downward_vel, max_fall_speed);
	}
}

void apply_physics(player* player, float delta_time) {
	player->position[0] = player->position[0] + delta_time * player->velocity[0];
	player->position[1] = player->position[1] + delta_time * player->velocity[1];
	quad_set_pos(&player->quad, player->position);
}
