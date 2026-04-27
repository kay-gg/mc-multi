#include "raylib.h"
#include "raymath.h"
#include "cmath"

#include<gen.hpp>
#include<chunk.hpp>
#include<world.hpp>
#include<chunk.hpp>


Color blockColor(BlockType type) {
    switch (type) {
        case STONE:   return GRAY;
        case DIRT:    return BROWN;
        case BEDROCK: return DARKGRAY;
        default:      return WHITE;
    }
}
void visuals(World* world) {
    InitWindow(1280, 720, "World Gen");
    SetTargetFPS(60);

    // lock and hide the cursor
    DisableCursor();

    Vector3 position = { 8.0f, 390.0f, 8.0f };
    float yaw   = 0.0f;   // left/right look angle
    float pitch = 0.0f;   // up/down look angle
    float speed = 20.0f;
    float sensitivity = 0.1f;
	DisableCursor();

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // toggle cursor with escape
        if (IsKeyPressed(KEY_ESCAPE)) {
            if (IsCursorHidden()) EnableCursor();
            else DisableCursor();
        }

        // mouse look (only when cursor is locked)
        if (IsCursorHidden()) {
            Vector2 mouseDelta = GetMouseDelta();
            yaw   += mouseDelta.x * sensitivity;
            pitch -= mouseDelta.y * sensitivity;
            pitch  = Clamp(pitch, -89.0f, 89.0f);
        }

        // calculate forward and right vectors from yaw
        float yawRad   = yaw   * DEG2RAD;
        float pitchRad = pitch * DEG2RAD;

        Vector3 forward = {
            cosf(pitchRad) * sinf(yawRad),
            sinf(pitchRad),
            cosf(pitchRad) * cosf(yawRad)
        };
        Vector3 right = {
            cosf(yawRad),
            0.0f,
            -sinf(yawRad)
        };

        // WASD movement
        if (IsKeyDown(KEY_W)) position = Vector3Add(position, Vector3Scale(forward, speed * dt));
        if (IsKeyDown(KEY_S)) position = Vector3Subtract(position, Vector3Scale(forward, speed * dt));
        if (IsKeyDown(KEY_D)) position = Vector3Add(position, Vector3Scale(right, speed * dt));
        if (IsKeyDown(KEY_A)) position = Vector3Subtract(position, Vector3Scale(right, speed * dt));

        // up/down with space and shift
        if (IsKeyDown(KEY_SPACE))      position.y += speed * dt;
        if (IsKeyDown(KEY_LEFT_SHIFT)) position.y -= speed * dt;

        Camera3D camera = {};
        camera.position   = position;
        camera.target     = Vector3Add(position, forward);
        camera.up         = { 0.0f, 1.0f, 0.0f };
        camera.fovy       = 90.0f;
        camera.projection = CAMERA_PERSPECTIVE;

        BeginDrawing();
        ClearBackground(SKYBLUE);

        BeginMode3D(camera);
            for (Chunk* chunk : world->getAllChunks()) {
                if (chunk->state != ChunkState::DONE) continue;

                float worldX = chunk->cords.x_pos * Chunk::SIZE_X;
                float worldZ = chunk->cords.z_pos * Chunk::SIZE_Z;

                for (int x = 0; x < Chunk::SIZE_X; x++)
                for (int y = 0; y < Chunk::SIZE_Y; y++)
                for (int z = 0; z < Chunk::SIZE_Z; z++) {
                    Block& block = chunk->block_arr[x][y][z];
                    if (block.type == AIR) continue;

                    Vector3 pos = {
                        worldX + x + 0.5f,
                        (float)y + 0.5f,
                        worldZ + z + 0.5f
                    };
                    DrawCube(pos, 1.0f, 1.0f, 1.0f, blockColor(block.type));
					DrawCubeWires(pos, 1.0f, 1.0f, 1.0f, BLACK);
                }
            }
        EndMode3D();

        DrawFPS(10, 10);
        if (IsCursorHidden())
            DrawText("ESC to release mouse", 10, 30, 20, WHITE);
        else
            DrawText("ESC to lock mouse", 10, 30, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();
}