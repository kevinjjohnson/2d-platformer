#include "include/level.h"
#include <stdio.h>

void save_level(char* filepath, level l) {
	FILE* level_file;
	fopen_s(&level_file, filepath, "wb");
	if (level_file == NULL) {
		printf("could not open level file to save: %s", filepath);
		return;
	}
	//write file id
	fwrite(&l.level_id, sizeof(int), 1, level_file);

	//write level spawnpoints
	fwrite(&l.num_spawns, sizeof(int), 1, level_file);

	fclose(level_file);
}

void load_level(char* filepath)
{
	FILE* level_file;
	fopen_s(&level_file, filepath, "rb");
	if (level_file == NULL) {
		printf("could not open level file to load: %s", filepath);
		return;
	}
	int id;
	int num_spawns;
	
	fread(&id, sizeof(int), 1, level_file);
	printf("Level id is: %d\n", id);

	fread(&num_spawns, sizeof(int), 1, level_file);
	printf("num of spawns is: %d\n", num_spawns);

	fclose(level_file);
}

void write_quad(char* filepath, quad q) {
	FILE* file;
	fopen_s(&file, filepath, "wb");

	if (file == NULL) {
		printf("error opening level file");
		return;
	}
	fwrite(q.size, sizeof(q.size), 1, file);
	fclose(file);
}

void read_quad(char* filepath) {
	FILE* file;
	fopen_s(&file, filepath, "rb");

	if (file == NULL) {
		printf("error opening level file");
		return;
	}
	vec2 data;
	fread(data, sizeof(vec2), 1, file);
	printf("READ DATA: %f, %f\n", data[0], data[1]);

}
