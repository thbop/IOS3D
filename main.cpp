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
    Color color;

    Obj(Vector3 p, Vector3 v, float d, float r) {
        density = d;
        radius = r;
        calcMass();

        pos = p;
        vel = v;
        color = GREEN;
    }
    Obj(Vector3 p, Vector3 v, float d, float r, Color c) {
        density = d;
        radius = r;
        calcMass();

        pos = p;
        vel = v;
        color = c;
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
        DrawSphereEx(pos, radius, 64, 64, color);
    }

};


Camera camera = { 0 };

std::list<Obj*> objs;



int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib");

    camera.position = (Vector3){ 300.0f, 10.0f, 8.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    objs.push_back(new Obj( {0}, {0}, 1e10, 50, YELLOW ));

    Obj *earth = new Obj( {0, 0, 300}, {0, 5, 1}, 1e3, 5, GREEN );
    objs.push_back(earth);


    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        UpdateCamera(&camera, CAMERA_THIRD_PERSON);
        // camera.target = earth->pos;
        // camera.position = Vector3Add( earth->pos, { 10.0f, 10.0f, 8.0f } );

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

    for ( Obj *o : objs ) delete o;
    CloseWindow();

    return 0;
}
