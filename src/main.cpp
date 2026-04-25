#include "raylib.h"
#include "world.hpp"
#include "chunk.hpp"
#include "gen.hpp"

void test_raylib() {
	InitWindow(1280, 720, "World Gen");

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawText("chunks go here!", 10, 10, 20, DARKGRAY);
		EndDrawing();
	}

    CloseWindow();
}

void report_stuff() {
	// Make 2 worlds
	// TODO
	int max_threads; // Get the max threads on the PC

	World* world_slow = new World(max_threads, SLOW);
	World* world_fast = new World(max_threads, FAST);

	// Generate enough chunks for spawn (like mc does)
	// Time those chunk generation times. (like 5 times so we can get an average)
	// Put times into a file
	// Run python program to graph the times in the file? 

	
}

void visual() {
	// all the stuff with raylib.


}

int main() {
	test_raylib();

	report_stuff();
	visual();
}