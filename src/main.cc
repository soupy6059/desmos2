// display complex functions

#include <iostream>
#include <vector>
#include <cassert>
#include <functional>
#include <exception>
#include <array>

#include <complex>
#include "craylib.h"
#include "event_loop.h"

int main() {
    const std::size_t screen_scale = 1920/16;
    const std::size_t screen_width = 16*screen_scale;
    const std::size_t screen_height = 9*screen_scale;

    rl::InitWindow(screen_width, screen_height, "hello from cpp");

    rl::Camera3D camera = {
        .position = { 10.0f, 10.0f, 10.0f },
        .target = { 0.0f, 0.0f, 0.0f },
        .up = { 0.0f, 1.0f, 0.0f },
        .fovy = 45.0f,
        .projection = rl::CAMERA_PERSPECTIVE,
    };
    float t = 0.f;

    rl::DisableCursor();
    rl::SetTargetFPS(60);

    while (!rl::WindowShouldClose()) {
        rl::UpdateCamera(&camera, rl::CAMERA_FREE);
        if (rl::IsKeyPressed(rl::KEY_Z)) camera.target = { 0.0f, 0.0f, 0.0f };
        if(rl::IsKeyDown(rl::KEY_F)) t += rl::GetFrameTime() * 0.1f;
        if(rl::IsKeyDown(rl::KEY_G)) t -= rl::GetFrameTime() * 0.1f;
        rl::BeginDrawing();
            rl::ClearBackground(rl::RAYWHITE);

            rl::BeginMode3D(camera);
                //test.identity().draw();
                event_loop(t);
            rl::EndMode3D();
            // display help
            if constexpr (true) {
                rl::DrawRectangle( 10, 10, 320, 93, rl::Fade(rl::SKYBLUE, 0.5f));
                rl::DrawRectangleLines( 10, 10, 320, 93, rl::BLUE);
                rl::DrawText("Free camera default controls:", 20, 20, 10, rl::BLACK);
                rl::DrawText("- Mouse Wheel to Zoom in-out", 40, 40, 10, rl::DARKGRAY);
                rl::DrawText("- Mouse Wheel Pressed to Pan", 40, 60, 10, rl::DARKGRAY);
                rl::DrawText("- Z to zoom to (0, 0, 0)", 40, 80, 10, rl::DARKGRAY);
            }
            // end display help
        rl::EndDrawing();
    }

    rl::CloseWindow();

    return 0;
}
