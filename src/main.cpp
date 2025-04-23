#include "headers/engine.h"
#include "headers/sceneObject.h"
#include "headers/presets.h"
#include "headers/mesh.h"
#include <iostream>

SceneObject floorPlane;
SceneObject ship;


void inputHandler() {
    if (IsKeyDown(KEY_W)) ship.pos.z += 0.20f;
    if (IsKeyDown(KEY_S)) ship.pos.z -= 0.20f; 
    if (IsKeyDown(KEY_D)) ship.pos.x += 0.20f;
    if (IsKeyDown(KEY_A)) ship.pos.x -= 0.20f;
    
    if (IsKeyDown(KEY_UP)) ship.rot.x += 0.015f;
    if (IsKeyDown(KEY_DOWN)) ship.rot.x -= 0.015f;
    if (IsKeyDown(KEY_LEFT)) ship.rot.z += 0.015f;
    if (IsKeyDown(KEY_RIGHT)) ship.rot.z -= 0.015f;


    if (IsKeyDown(KEY_LEFT_SHIFT)) ship.pos.y -= 0.20f;
    if (IsKeyDown(KEY_LEFT_CONTROL)) ship.pos.y += 0.20f;

    if (IsKeyPressed(KEY_F11)) {
        if (!IsWindowFullscreen()) {
            int monitor = GetCurrentMonitor();
            SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
            ToggleFullscreen();
        }
        else {
            int monitor = GetCurrentMonitor();
            SetWindowSize(1000, 700);
            ToggleFullscreen();
        }
    }
}

int main(void)  {
    //Class Instance
    Engine engine;

    ship.loadMeshFromObjectFile("../meshes/ship.obj");
    floorPlane.loadMeshFromObjectFile("../meshes/plane.obj");

    floorPlane.pos.z = 15.0f;
    floorPlane.pos.y = 6;

    ship.pos.z = 15.0f;
    ship.pos.y = 3.0f;


    Vec3D t1(2, 0, 1);
    Vec3D t2(3, 2, -2);

    t1 *= engine.simpleProjection;
    std::cout << t1.z << std::endl;
    

    while (!WindowShouldClose()) {
        engine.width = GetScreenWidth();
        engine.height = GetScreenHeight();
        engine.aspectRatio = (float)engine.height/(float)engine.width;

        inputHandler();

        BeginDrawing();
            ClearBackground(BLACK);
            
            engine.drawMesh(engine.perspective, ship);
            engine.drawMesh(engine.perspective, floorPlane);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

