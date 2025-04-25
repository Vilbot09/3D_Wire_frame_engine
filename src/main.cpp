#include "headers/engine.h"
#include "headers/sceneObject.h"
#include "headers/mesh.h"
#include "headers/cameraObject.h"
#include <iostream>

SceneObject floorPlane;
SceneObject ship;


void inputHandler(Engine engine, CameraObject &camera) {
    if (IsKeyDown(KEY_RIGHT)) camera.fYaw -= camera.fYawSpeed;
    if (IsKeyDown(KEY_LEFT)) camera.fYaw += camera.fYawSpeed;
    if (IsKeyDown(KEY_UP)) camera.fPitch -= camera.fPitchSpeed;
    if (IsKeyDown(KEY_DOWN)) camera.fPitch += camera.fPitchSpeed;

    Vec3D forwardVector = camera.vLookDir * camera.fSpeed;
    Vec3D rightVector = engine.vUp.Product_Cross(camera.vLookDir) * camera.fSpeed;

    if (IsKeyDown(KEY_W)) camera.vPos += forwardVector;
    if (IsKeyDown(KEY_S)) camera.vPos -= forwardVector; 
    if (IsKeyDown(KEY_D)) camera.vPos += rightVector;
    if (IsKeyDown(KEY_A)) camera.vPos -= rightVector;



    /*
    if (IsKeyDown(KEY_UP)) ship.vRot.x += 0.015f;
    if (IsKeyDown(KEY_DOWN)) ship.vRot.x -= 0.015f;
    if (IsKeyDown(KEY_LEFT)) ship.vRot.z += 0.015f;
    if (IsKeyDown(KEY_RIGHT)) ship.vRot.z -= 0.015f;
    */

    if (IsKeyDown(KEY_LEFT_SHIFT)) camera.vPos.y += 0.20f;
    if (IsKeyDown(KEY_SPACE)) camera.vPos.y -= 0.20f;
    

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

    floorPlane.vPos.z = 15.0f;
    floorPlane.vPos.y = 6;
    ship.vPos.z = 15.0f;
    ship.vPos.y = 3.0f;
    

    while (!WindowShouldClose()) {
        engine.camera.fAspectRatio = (float)engine.camera.Get_Height()/(float)engine.camera.Get_Width();
        
        ship.vRot.y += 0.01f;

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

