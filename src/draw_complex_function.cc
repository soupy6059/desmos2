#include "draw_complex_function.h"
#include "craylib.h"

using namespace dcf;

dcf::settings dcf::defaults::make() { return {
    .input_bounds = defaults::bound,
    .epsilon = defaults::epsilon,
    .height = defaults::height,
    .color = defaults::color,
    .lighting = defaults::lighting,
};}

void compute_func(C_to_C func, R x, R y, std::array<C,4> &zs, const settings &setting) {
    using namespace std::complex_literals;
    zs = {
        func(x+y*1if),
        func((x+setting.epsilon)+y*1if),
        func(x+(y+setting.epsilon)*1if),
        func((x+setting.epsilon)+(y+setting.epsilon)*1if)
    };
}

void compute_func(C_to_C2 func, R x, R y, std::array<C2,4> &zs, const settings &setting) {
    using namespace std::complex_literals;
    zs = {
        func(x+y*1if),
        func((x+setting.epsilon)+y*1if),
        func(x+(y+setting.epsilon)*1if),
        func((x+setting.epsilon)+(y+setting.epsilon)*1if)
    };
}

void compute_triangle_corners(R x, R y, const std::array<C,4> &zs, std::array<rl::Vector3,4> &triangle_corners, const settings &setting) {
    triangle_corners = {
        rl::Vector3{x, y, setting.height(zs[0])},
        rl::Vector3{x+setting.epsilon, y, setting.height(zs[1])},
        rl::Vector3{x, y+setting.epsilon, setting.height(zs[2])},
        rl::Vector3{x+setting.epsilon, y+setting.epsilon, setting.height(zs[3])}
    };
}

void compute_triangle_corners(
    R src_x[[maybe_unused]], R src_y[[maybe_unused]], const std::array<C2,4> &z2s,
    std::array<rl::Vector3,4> &triangle_corners, const settings &setting) {
    const std::array<R,4> xs = { std::real(z2s[0].first),std::real(z2s[1].first),std::real(z2s[2].first),std::real(z2s[3].first) };
    const std::array<R,4> ys = { std::imag(z2s[0].first),std::imag(z2s[1].first),std::imag(z2s[2].first),std::imag(z2s[3].first) };
    const std::array<R,4> zs = { setting.height(z2s[0].second),setting.height(z2s[1].second),setting.height(z2s[2].second),setting.height(z2s[3].second) };
    rl::Vector3 p00 {xs[0],ys[0],zs[0]};
    rl::Vector3 p10 {xs[1],ys[1],zs[1]};
    rl::Vector3 p01 {xs[2],ys[2],zs[2]};
    rl::Vector3 p11 {xs[3],ys[3],zs[3]};
    triangle_corners = {
        p00, p10, p01, p11
        //rl::Vector3{x, y, setting.height(zs[0])},
        //rl::Vector3{x+setting.epsilon, y, setting.height(zs[1])},
        //rl::Vector3{x, y+setting.epsilon, setting.height(zs[2])},
        //rl::Vector3{x+setting.epsilon, y+setting.epsilon, setting.height(zs[3])}
    };
}

void compute_lighting(const std::array<rl::Vector3,4> &triangle_corners, rl::Color &triangle_colors, const settings &setting[[maybe_unused]]) {
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

void dcf::draw_complex_function(C_to_C func, settings setting) {
    using namespace std::complex_literals;
    for(R x {std::get<0>(setting.input_bounds)}; x < std::get<1>(setting.input_bounds); x += setting.epsilon) {
        for(R y{std::get<0>(setting.input_bounds)}; y < std::get<1>(setting.input_bounds); y += setting.epsilon) {
            constexpr static const std::size_t scn = 4; // square corner number
            std::array<C,scn> zs;
            std::array<rl::Vector3,scn> triangle_corners; 
            rl::Color triangle_colors = rl::RED;
            
            compute_func(func, x, y, zs, setting);
            compute_triangle_corners(x, y, zs, triangle_corners, setting);
            triangle_colors = setting.color(zs[0]);

            if(setting.lighting == settings::LIGHTING::ON) compute_lighting(triangle_corners, triangle_colors, setting);
            
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

void dcf::draw_complex_function(C_to_C2 func, settings setting) {
using namespace std::complex_literals;
for(R x {std::get<0>(setting.input_bounds)}; x < std::get<1>(setting.input_bounds); x += setting.epsilon) {
for(R y{std::get<0>(setting.input_bounds)}; y < std::get<1>(setting.input_bounds); y += setting.epsilon) {
    constexpr static const std::size_t scn = 4; // square corner number
    std::array<C2,scn> z2s;
    std::array<rl::Vector3,scn> triangle_corners; 
    rl::Color triangle_colors = rl::RED;
    
    compute_func(func, x, y, z2s, setting);
    compute_triangle_corners(x, y, z2s, triangle_corners, setting);
    triangle_colors = setting.color(z2s[0].second);

    if(setting.lighting == settings::LIGHTING::ON) compute_lighting(triangle_corners, triangle_colors, setting);
    
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

