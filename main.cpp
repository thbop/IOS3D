#include "raylib.h"
#include "raymath.h"
#include <list>
#include <iostream>

#define G 6.67e-11f

Vector3 Vector3DivideValue( Vector3 p, float v )   { return (Vector3){ p.x / v, p.y / v, p.z / v }; }
Vector3 Vector3MultiplyValue( Vector3 p, float v ) { return (Vector3){ p.x * v, p.y * v, p.z * v }; }

class Obj {
public:
    float density, radius, mass;
    Vector3 pos, vel;

    Obj(Vector3 p, Vector3 v, float d, float r) {
        density = d;
        radius = r;
        calcMass();

        pos = p;
        vel = v;
    }

    void calcMass() {
        // 4/3 * pi * r^2 * density
        mass = 4.18879f * radius*radius * density;
    }

    void move() {
        pos = Vector3Add(pos, vel);
    }

    void gravity(Obj other) {
        // float Fmag = ( G * mass * other.mass ) / ( Vector3DistanceSqr(pos, other.pos) );
        // Vector3 Fdir = Vector3DivideValue( Vector3Subtract( other.pos, pos ), Vector3Distance( pos, other.pos ) );
        // Vector3 F = Vector3MultiplyValue( Fdir, Fmag );
        float Amag = ( G * other.mass ) / ( Vector3DistanceSqr(pos, other.pos) );
        Vector3 Adir = Vector3DivideValue( Vector3Subtract( other.pos, pos ), Vector3Distance( pos, other.pos ) );
        Vector3 A = Vector3MultiplyValue( Adir, Amag );

        vel = Vector3Add( vel, A );
        // std::cout << vel.x << " " << vel.y << " " << vel.z << "\n";
    }

    void draw() {
        DrawSphereEx(pos, radius, 64, 64, RED);
    }

};


Camera camera = { 0 };

std::list<Obj*> objs;


static void UpdateDrawFrame();


int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib");

    camera.position = (Vector3){ 100.0f, 10.0f, 8.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    objs.push_back(new Obj( {0}, {0}, 1e10, 3 ));
    objs.push_back(new Obj( {50, 0, 0}, {0, 0.7f, 0.4f}, 1e3, 3 ));


    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        UpdateDrawFrame();
    }

    for ( Obj *o : objs ) delete o;
    CloseWindow();

    return 0;
}

// Update and draw game frame
static void UpdateDrawFrame(void) {

    UpdateCamera(&camera, CAMERA_THIRD_PERSON);

    BeginDrawing();

        ClearBackground(BLACK);

        BeginMode3D(camera);
            for ( Obj *o : objs ) {
                for ( Obj *oo : objs ) { if ( oo != o ) {
                    o->gravity(*oo);
                }}
                o->move();
                o->draw();
            }

            DrawGrid(256, 64);

        EndMode3D();

    EndDrawing();

}
