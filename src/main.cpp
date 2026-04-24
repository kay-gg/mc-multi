#include "raylib.h"

void test() {
	InitWindow(800, 600, "World Gen");

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawText("chunks go here!", 10, 10, 20, DARKGRAY);
		EndDrawing();
	}

    CloseWindow();
}

int main() {
	test();
}