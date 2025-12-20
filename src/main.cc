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
        .up = { 0.0f, 0.0f, 1.0f },
        .fovy = 45.0f,
        .projection = rl::CAMERA_PERSPECTIVE,
    };
    float t = 1.f;
    float eps = 0.1f;
    rl::Vector2 cord = {0.f,0.f};

    rl::DisableCursor();
    rl::SetTargetFPS(60);

    while (!rl::WindowShouldClose()) {
        rl::UpdateCamera(&camera, rl::CAMERA_FREE);
        if (rl::IsKeyPressed(rl::KEY_Z)) camera.target = { 0.0f, 0.0f, 0.0f };

        // time parameter controls
        if(rl::IsKeyDown(rl::KEY_F)) t += rl::GetFrameTime() * 0.1f;
        if(rl::IsKeyDown(rl::KEY_G)) t -= rl::GetFrameTime() * 0.1f;

        // cord controls
        const float speed = 0.5f;
        if(rl::IsKeyDown(rl::KEY_H)) cord.x += rl::GetFrameTime() * speed;
        if(rl::IsKeyDown(rl::KEY_J)) cord.y -= rl::GetFrameTime() * speed;
        if(rl::IsKeyDown(rl::KEY_K)) cord.y += rl::GetFrameTime() * speed;
        if(rl::IsKeyDown(rl::KEY_L)) cord.x -= rl::GetFrameTime() * speed;

        // eps controls
        if(rl::IsKeyDown(rl::KEY_X)) eps += rl::GetFrameTime() * .1f;
        if(rl::IsKeyDown(rl::KEY_C)) eps -= rl::GetFrameTime() * .1f;
        eps = std::max(0.005f,eps);

        rl::BeginDrawing();
            rl::ClearBackground(rl::RAYWHITE);

            rl::BeginMode3D(camera);
                //test.identity().draw();
                rl::DrawSphere(rl::Vector3{1.f,0.f,0.f}, .1f, rl::RED);
                event_loop(t,cord,eps);
            rl::EndMode3D();

            // debug info
            if constexpr(true) {
                int text_pos = 10;
                rl::DrawFPS(10,text_pos += 20);
                rl::DrawText(("t: " + std::to_string(t)).c_str(), 10, text_pos += 20, 20, rl::BLUE);
                rl::DrawText(("{" + std::to_string(camera.position.x) + ", " + std::to_string(camera.position.y) + ", " +
                        std::to_string(camera.position.z) + "}").c_str(), 10, text_pos += 20, 20, rl::BLUE);
                rl::DrawText(("eps: " + std::to_string(eps)).c_str(), 10, text_pos += 20, 20, rl::BLUE);
            }
            // end debug info

            // display help
            if constexpr(false) {
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
