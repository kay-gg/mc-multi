#include "raylib.h"
#include "world.hpp"
#include "chunk.hpp"
#include "gen.hpp"
#include "visuals.hpp"
#include "timing.hpp"
#include <string>

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
	int max_threads = std::thread::hardware_concurrency();; // Get the max threads on the PC
    std::cout << "Using " << max_threads << " threads" << std::endl;

	World* world_fast = new World(max_threads, std::make_unique<FlatWorldGen>());
	World* world_slow = new World(max_threads, std::make_unique<FlatWorldGenSequential>());

	// Generate enough chunks for spawn (like mc does)
	// 19x19 is the spawn area
	for (int i = 0; i < 19; i++)
	for (int j = 0; j < 19; j++)
	{
		world_fast->genChunk(ChunkCord {i, j});
	}

	// Time those chunk generation times. (like 5 times so we can get an average)

	// Put times into a file

	// Run python program to graph the times in the file? 

}
void debugging() {
	int max_threads = std::thread::hardware_concurrency();; // Get the max threads on the PC
    std::cout << "Using " << max_threads << " threads" << std::endl;

	// World* world_fast = new World(max_threads, std::make_unique<FlatWorldGen>());
	// World* world_slow = new World(max_threads, std::make_unique<FlatWorldGenSequential>());
	World* world_debug = new World(max_threads, std::make_unique<FlatWorldGenDebug>());

	world_debug->genChunk(ChunkCord {0, 0});
	world_debug->debugChunk(ChunkCord {0, 0});
}

	int main(int argc, char* argv[]) {
    if (argc > 1 && std::string(argv[1]) == "timing") {
        runTimingReport();
        return 0;
    }
	//test_raylib();
	report_stuff();
	//debugging();
	int max_threads = std::thread::hardware_concurrency();; // Get the max threads on the PC
	World* world_slow = new World(max_threads, std::make_unique<FlatWorldGen>());
	world_slow->genChunk(ChunkCord {0, 0});
	world_slow->genChunk(ChunkCord {0, 1});
	world_slow->genChunk(ChunkCord {1, 0});
	world_slow->genChunk(ChunkCord {1, 1});
	//visuals(world_slow);
}