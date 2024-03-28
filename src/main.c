#include "e2d_core.h"

typedef struct vehicle_t {
    Vector2 location;
    Vector2 velocity;
    Vector2 accel;
    float radius;
    float max_steer;
    float max_desired;
} Vehicle;

typedef struct path_t {
    float radius;
    Vector2 start;
    Vector2 end;
} Path;

Vector2 getNormalPoint(Vector2 position, Vector2 a, Vector2 b) {
    Vector2 a_to_position= e2_VecSub(position, a);
    Vector2 a_to_b = e2_VecSub(b, a);
    e2_vecNorm(&a_to_b);
    float sp = e2_dot(a_to_position, a_to_b);
    e2_vecScale(&a_to_b, sp);
    Vector2 result = e2_VecAdd(a, a_to_b);
    return result;
}

void applyForce(Vehicle *vehic, Vector2 force) {
    vehic->accel = e2_VecAdd(vehic->accel, e2_VecDiv(force, vehic->radius*2));
}

Vehicle createVehicle(float x, float y, float r) {
    Vehicle result = {
        .location = {x, y},
        .radius = r,
        .velocity = {0, 0},
        .accel = {0, 0},
        .max_steer = 8.0f,
        .max_desired = 8.0f
        };
    return result;
} 

Path createPath() {
    Path result = {
        .radius = 30,
        .start = {0, 400},
        .end = {1600, 400} 
    };
    return result;
}

void showPath(Path* p) {
    DrawLineEx(p->start, p->end, p->radius * 2, GRAY);
    DrawLineEx(p->start, p->end, 2, YELLOW);

}

void followPath(Vehicle* vehic, Path* path) {
    Vector2 future = vehic->velocity;
    e2_vecSetMag(&future, 40);
    e2_vecAdd(&future, vehic->location); //vehicles future location
    Vector2 normalPoint = getNormalPoint(vehic->location, path->start, path->end);
    //move a little further along the path
    Vector2 pathline = e2_VecSub(path->end, path->start);
    e2_vecSetMag(&pathline, 40);
    Vector2 target = e2_VecAdd(normalPoint, pathline);

}

void updateVehicle(Vehicle* vehic) {
    vehic->velocity = e2_VecAdd(vehic->velocity, vehic->accel);
    e2_vecLimit(&vehic->velocity, 10.0f);
    e2_vecSet(&vehic->accel, 0.0f, 0.0f);
    vehic->location = e2_VecAdd(vehic->location, vehic->velocity);
}

void drawVehicle(Vehicle* vehic, Color c1, Color c2) {
    Vector2 orientation = vehic->velocity;
    e2_vecSetMag(&orientation, vehic->radius * 1.4f);
    DrawRing(vehic->location, vehic->radius - 3, vehic->radius, 0, 360, 1, c1);
    DrawLineEx(vehic->location, e2_VecAdd(vehic->location, orientation), 2, c2);
}

int main() {
    const int screenWidth = 1600;
    const int screenHeight = 1200; 

    InitWindow(screenWidth, screenHeight, "seeking and arrive");
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    Vehicle mover = createVehicle(400, 200, 20);
    mover.velocity = (Vector2){4, 1};
    Path path = createPath();

    // Main game loop
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKBLUE);
        showPath(&path);
        drawVehicle(&mover, YELLOW, RED);
        updateVehicle(&mover);
    	EndDrawing();
    }

    CloseWindow();   

    return EXIT_SUCCESS;
}
