#include "draw_complex_function.h"
#include "craylib.h"

// todo: factor into input structure

void draw_complex_function::draw() {
    using namespace std::complex_literals;
    for(float x {std::get<0>(input_bounds)}; x < std::get<1>(input_bounds); x += eps) {
        for(float y{std::get<0>(input_bounds)}; y < std::get<1>(input_bounds); y += eps) {
            constexpr static const std::size_t square_corner_number = 4;
            std::array<std::complex<float>,square_corner_number> zs;
            std::array<rl::Vector3,square_corner_number> triangle_corners; 
            rl::Color triangle_colors = rl::RAYWHITE;
            
            zs = {
                func(x+y*1if),
                func((x+eps)+y*1if),
                func(x+(y+eps)*1if),
                func((x+eps)+(y+eps)*1if)
            };

            if((DRAW_SETTINGS&(INPUT|OUTPUT)) == (CARTESION_IN|RADIAL_OUT)) {
                triangle_corners = {
                    rl::Vector3{x, y, height_func(zs[0])},
                    rl::Vector3{x+eps, y, height_func(zs[1])},
                    rl::Vector3{x, y+eps, height_func(zs[2])},
                    rl::Vector3{x+eps, y+eps, height_func(zs[3])}
                };
                triangle_colors = color_func(zs[0]);
            }

            if((DRAW_SETTINGS & LIGHTING) == LIGHTING_ON) {
                auto hsv = rl::ColorToHSV(triangle_colors);
                const rl::Vector3 light_direction {rl::Vector3Normalize({0.f,0.f,1.f})};
                auto d0 = rl::Vector3Subtract(triangle_corners[1], triangle_corners[0]);
                auto d1 = rl::Vector3Subtract(triangle_corners[2], triangle_corners[0]);
                auto normal = rl::Vector3CrossProduct(d0,d1);
                auto unit_normal = rl::Vector3Normalize(normal); 
                auto value = rl::Vector3DotProduct(unit_normal, light_direction);
                value = rl::Remap(value, -1.f, 1.f, 0.f, 1.f); // move bounds linearly
                triangle_colors = rl::ColorFromHSV(hsv.x, hsv.y, value);
                if constexpr(false) {
                    auto average = rl::Vector3Add(rl::Vector3Add(triangle_corners[0],triangle_corners[1]),triangle_corners[2]);
                    average = rl::Vector3Scale(average, 1.f/3.f);
                    rl::DrawLine3D(rl::Vector3Zero(), light_direction, rl::BLUE);
                    rl::DrawLine3D(average, rl::Vector3Add(average,unit_normal), rl::BLUE);
                }
            }
            
            rl::DrawTriangle3D(
                triangle_corners[0],
                triangle_corners[1],
                triangle_corners[2],
                triangle_colors
            );
            rl::DrawTriangle3D(
                triangle_corners[3],
                triangle_corners[2],
                triangle_corners[1],
                triangle_colors
            );
        }
    }
}

