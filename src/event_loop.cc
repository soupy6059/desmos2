#include "event_loop.h"
#include "craylib.h"
#include "draw_complex_function.h"

#include <complex>
#include <map>

void event_loop(float t, float t2, rl::Vector2 cord, float eps, float bound_radius) {
    using namespace std::complex_literals;
    using namespace dcf;

    static C_to_C f {
    [&t](C z) -> C {
        return std::atan(z);
    }};

    dcf::settings s {defaults::make()};
    s.input_bounds = Bound{-bound_radius,bound_radius};
    s.epsilon = eps;
    static C_to_C2 g {
    [&t,&cord,&t2](C z) -> C2 {
        z -= C{cord.x,cord.y};
        return { std::pow(z,t2), std::pow(z,t) };
    }};

    dcf::draw_complex_function(
        g, s
    );

    //dcf::draw_complex_function(
        //f, defaults::make()
    //);
}
