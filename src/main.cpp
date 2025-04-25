#include "headers/engine.h"
#include "headers/sceneObject.h"
#include "headers/mesh.h"
#include "headers/cameraObject.h"
#include <iostream>

SceneObject floorPlane;
SceneObject ship;


void inputHandler(Engine engine, CameraObject &camera) {
    if (IsKeyDown(KEY_RIGHT)) camera.yaw -= camera.fYawSpeed;
    if (IsKeyDown(KEY_LEFT)) camera.yaw += camera.fYawSpeed;
    if (IsKeyDown(KEY_UP)) camera.pitch -= camera.fPitchSpeed;
    if (IsKeyDown(KEY_DOWN)) camera.pitch += camera.fPitchSpeed;

    Vec3D forwardVector = camera.lookDir * camera.fSpeed;
    Vec3D rightVector = engine.upVector.Product_Cross(camera.lookDir) * camera.fSpeed;

    if (IsKeyDown(KEY_W)) camera.pos += forwardVector;
    if (IsKeyDown(KEY_S)) camera.pos -= forwardVector; 
    if (IsKeyDown(KEY_D)) camera.pos += rightVector;
    if (IsKeyDown(KEY_A)) camera.pos -= rightVector;



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

    ship.LoadMeshFromObjectFile("../meshes/ship.obj");
    floorPlane.LoadMeshFromObjectFile("../meshes/plane.obj");

    floorPlane.pos.z = 15.0f;
    floorPlane.pos.y = 6;
    ship.pos.z = 15.0f;
    ship.pos.y = 3.0f;
    

    while (!WindowShouldClose()) {
        engine.camera.width = GetScreenWidth();
        engine.camera.height = GetScreenHeight();
        engine.camera.fAspectRatio = (float)engine.camera.height/(float)engine.camera.width;
        
        ship.rot.y += 0.01f;

        inputHandler(engine, engine.camera);

        BeginDrawing();
            ClearBackground(BLACK);
            
            engine.Render_SceneObject(ship);
            engine.Render_SceneObject(floorPlane);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

