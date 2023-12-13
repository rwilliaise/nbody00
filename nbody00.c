
#include <stdint.h>
#include <stdio.h>
#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define G_CONST 0.0000000000667428

typedef struct {
    float radius;
    double mass;
    Vector3 pos;
    Vector3 vel;
} body_t;

typedef struct {
    body_t *bodies;
    uint32_t body_count;
} world_t;

void body_update(body_t *B) {
        
}



int main(int argc, char *argv[]) {
    InitWindow(640, 480, "nbody00");

    double last_time = GetTime();

    while (!WindowShouldClose()) {
        double delta = GetTime() - last_time;
        last_time = GetTime();

        BeginDrawing();
            ClearBackground(WHITE);

            BeginMode3D((Camera3D) {
                .position = (Vector3) { 5, 2, 5 },
                .target = (Vector3) { 0, 0, 0 },
                .up = (Vector3) { 0, 1, 0 },
                .fovy = 90,
                .projection = CAMERA_PERSPECTIVE,
            });

            DrawLine3D(
                (Vector3) { -1.0f, sin(GetTime()), 0.0f },
                (Vector3) { 1.0f, cos(GetTime()), 0.0f },
                BLACK
            );


            EndMode3D();

        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}
