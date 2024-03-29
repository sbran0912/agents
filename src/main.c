#include "lib_core.h"

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
    Vector2 a_to_position= Vec2Sub(position, a);
    Vector2 a_to_b = Vec2Sub(b, a);
    a_to_b = Vec2Normalize(a_to_b);
    float sp = Vec2DotProduct(a_to_position, a_to_b);
    a_to_b = Vec2Scale(a_to_b, sp);
    Vector2 result = Vec2Add(a, a_to_b);
    return result;
}

void applyForce(Vehicle *vehic, Vector2 force) {
    vehic->accel = Vec2Add(vehic->accel, Vec2Div(force, vehic->radius*2));
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

void seek(Vehicle* vehic, Vector2 target) {
    //float zone = 300.0f;
    Vector2 desired = Vec2Sub(target, vehic->location);
    float distance = Vec2Mag(desired);
    desired = Vec2Scale(Vec2Normalize(desired), vehic->max_desired);
    Vector2 steer = Vec2Limit(Vec2Sub(desired, vehic->velocity), vehic->max_steer);
    applyForce(vehic, steer);
}

void showPath(Path* p) {
    DrawLineEx(p->start, p->end, p->radius * 2, GRAY);
    DrawLineEx(p->start, p->end, 2, YELLOW);

}

void followPath(Vehicle* vehic, Path* path) {
    Vector2 future = vehic->velocity;
    future = Vec2SetMag(future, 40);
    Vec2Add(future, vehic->location); //vehicles future location
    Vector2 normalPoint = getNormalPoint(vehic->location, path->start, path->end);
    //move a little further along the path
    Vector2 pathline = Vec2Sub(path->end, path->start);
    pathline = Vec2SetMag(pathline, 40);
    Vector2 target = Vec2Add(normalPoint, pathline);
    float distance = Vec2Dist(normalPoint, future);
    if (distance > path->radius) {
        seek(vehic, target);
    }

}

void updateVehicle(Vehicle* vehic) {
    vehic->velocity = Vec2Add(vehic->velocity, vehic->accel);
    vehic->velocity = Vec2Limit(vehic->velocity, 10.0f);
    vehic->accel =(Vector2){0.0f, 0.0f};
    vehic->location = Vec2Add(vehic->location, vehic->velocity);
}

void drawVehicle(Vehicle* vehic, Color c1, Color c2) {
    Vector2 orientation = vehic->velocity;
    orientation = Vec2SetMag(orientation, vehic->radius * 1.4f);
    DrawRing(vehic->location, vehic->radius - 3, vehic->radius, 0, 360, 1, c1);
    DrawLineEx(vehic->location, Vec2Add(vehic->location, orientation), 2, c2);
}

int main() {
    const int screenWidth = 1600;
    const int screenHeight = 1200; 

    InitWindow(screenWidth, screenHeight, "Following path");
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    Vehicle mover = createVehicle(400, 200, 20);
    mover.velocity = (Vector2){4, 0};
    Path path = createPath();

    // Main game loop
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKBLUE);
        showPath(&path);
        drawVehicle(&mover, YELLOW, RED);
        followPath(&mover, &path);
        updateVehicle(&mover);
    	EndDrawing();
    }

    CloseWindow();   

    return EXIT_SUCCESS;
}
