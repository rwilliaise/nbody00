
#include <stdint.h>
#include <stdio.h>
#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define G_CONST 6.674e-11
#define R_SCALE 1.0e-6

#define VEC3_ZERO (vec3d) { 0, 0, 0 }
#define dbg_vec(A) printf(#A ": %f, %f, %f\n", A.x, A.y, A.z);
#define dbgf(A) printf(#A ": %f\n", A);

typedef struct {
    double x;
    double y;
    double z;
} vec3d;

typedef struct {
    float radius;
    double mass;
    vec3d pos;
    vec3d vel;
    Color color;
} body_t;

typedef struct {
    body_t *bodies;
    uint32_t body_count;
    double delta;
} world_t;

void veci_add(vec3d *A, vec3d B) {
    A->x += B.x;
    A->y += B.y;
    A->z += B.z;
}

vec3d vec_add(vec3d A, vec3d B) {
    return (vec3d) { A.x + B.x, A.y + B.y, A.z + B.z };
}

vec3d vec_sub(vec3d A, vec3d B) {
    return (vec3d) { A.x - B.x, A.y - B.y, A.z - B.z };
}

vec3d vec_muls(vec3d A, float B) {
    return (vec3d) { A.x * B, A.y * B, A.z * B };
}

vec3d vec_divs(vec3d A, float B) {
    return (vec3d) { A.x / B, A.y / B, A.z / B };
}

float vec_dot(vec3d A, vec3d B) {
    return A.x * B.x + A.y * B.y + A.z * B.z;
}

float vec_length(vec3d A) {
    return sqrtf(vec_dot(A, A));
}

vec3d vec_normalized(vec3d A) {
    return vec_divs(A, vec_length(A));   
}

Vector3 vec_convert(vec3d A) {
    return (Vector3) { (float) A.x, (float) A.y, (float) A.z };
}

void body_render(body_t *B) {
    vec3d r_pos = vec_muls(B->pos, R_SCALE);
    vec3d r_end_pos = vec_add(r_pos, vec_muls(B->vel, R_SCALE));
    DrawLine3D(vec_convert(r_pos), vec_convert(r_end_pos), B->color);
    DrawSphere(vec_convert(r_pos), B->radius * R_SCALE, B->color);
}

void body_update(body_t *B, world_t *W) {
    vec3d F_net = VEC3_ZERO;
    for (int i = 0; i < W->body_count; i++) {
        body_t *other = &W->bodies[i];
        if (other == B) continue;
        vec3d diff = vec_sub(other->pos, B->pos);
        float dist = vec_length(diff);
        
        vec3d F = vec_muls(vec_divs(diff, dist), G_CONST * ((other->mass * B->mass) / dist));
        veci_add(&F_net, F);
    }

    veci_add(&B->vel, vec_muls(F_net, W->delta * (1.0 / B->mass)));
    veci_add(&B->pos, vec_muls(B->vel, W->delta));
}

void world_render(world_t *W) {
    for (int i = 0; i < W->body_count; i++) {
        body_render(&W->bodies[i]);
    }
}

void world_update(world_t *W) {
    for (int i = 0; i < W->body_count; i++) {
        body_update(&W->bodies[i], W);
    }
}

int main(int argc, char *argv[]) {
    InitWindow(1280, 720, "nbody00");

    double last_time = GetTime();

    world_t world;
    world.body_count = 2;
    world.bodies = malloc(world.body_count * sizeof(body_t));
    memcpy(
        &world.bodies[0],
        &(body_t) {
            .radius = 6378000.0,
            .mass = 5.972e24,
            .pos = VEC3_ZERO,
            .vel = VEC3_ZERO,
            .color = BLUE,
        },
        sizeof(body_t)
    );
    memcpy(
        &world.bodies[1],
        &(body_t) {
            .radius = 1740000.0,
            .mass = 7.348e22,
            .pos = (vec3d) { 384400000, 0, 0 },
            .vel = (vec3d) { 0, 0, 61383 },
            .color = GRAY,
        },
        sizeof(body_t)
    );

    while (!WindowShouldClose()) {
        world.delta = GetTime() - last_time;
        last_time = GetTime();

        BeginDrawing();
            ClearBackground(WHITE);

            BeginMode3D((Camera3D) {
                .position = (Vector3) { -30, 25, 30 },
                .target = vec_convert(vec_muls(world.bodies[1].pos, R_SCALE)),
                .up = (Vector3) { 0, 1, 0 },
                .fovy = 90,
                .projection = CAMERA_PERSPECTIVE,
            });

            world_update(&world);
            world_render(&world);

            EndMode3D();

        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}
