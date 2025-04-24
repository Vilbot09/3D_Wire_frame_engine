#include "headers/engine.h"
#include "headers/sceneObject.h"
#include "headers/presets.h"
#include "headers/mesh.h"
#include "headers/cameraObject.h"
#include <iostream>

SceneObject floorPlane;
SceneObject ship;


void inputHandler(CameraObject &camera) {
    if (IsKeyDown(KEY_RIGHT)) camera.yaw -= 0.01f;
    if (IsKeyDown(KEY_LEFT)) camera.yaw += 0.01f;

    Vec3D fowardVector = camera.lookDir * 0.2f;

    if (IsKeyDown(KEY_W)) camera.pos += fowardVector;
    if (IsKeyDown(KEY_S)) camera.pos -= fowardVector; 
    //if (IsKeyDown(KEY_D)) camera.pos.x += 0.20f;
    //if (IsKeyDown(KEY_A)) camera.pos.x -= 0.20f;



    /*
    if (IsKeyDown(KEY_UP)) ship.rot.x += 0.015f;
    if (IsKeyDown(KEY_DOWN)) ship.rot.x -= 0.015f;
    if (IsKeyDown(KEY_LEFT)) ship.rot.z += 0.015f;
    if (IsKeyDown(KEY_RIGHT)) ship.rot.z -= 0.015f;
    */

    if (IsKeyDown(KEY_LEFT_SHIFT)) camera.pos.y += 0.20f;
    if (IsKeyDown(KEY_SPACE)) camera.pos.y -= 0.20f;
    

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
    

    while (!WindowShouldClose()) {
        engine.camera.width = GetScreenWidth();
        engine.camera.height = GetScreenHeight();
        engine.camera.fAspectRatio = (float)engine.camera.height/(float)engine.camera.width;
        
        ship.rot.y += 0.01f;

        inputHandler(engine.camera);

        BeginDrawing();
            ClearBackground(BLACK);
            
            engine.drawObject(ship);
            engine.drawObject(floorPlane);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

