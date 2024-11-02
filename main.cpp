#include <iostream>
#include "raylib.h"
#include "raymath.h"

#define WIDTH 1200
#define HEIGHT 800

void update() {

}

void draw() {

}

int main() {
    InitWindow( WIDTH, HEIGHT, "IOS3D" );
    SetTargetFPS(60);

    while(!WindowShouldClose()) {
        update();

        BeginDrawing();
            ClearBackground(BLACK);
            draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}