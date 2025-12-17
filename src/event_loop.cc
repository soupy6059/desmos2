#include "event_loop.h"
#include "craylib.h"
#include "draw_complex_function.h"

#include <complex>
#include <map>

void event_loop(float t) {
    ::draw_complex_function(
        [&t](std::complex<float> z) -> std::complex<float> {
            using namespace std::complex_literals;
            return std::exp(-1.f * std::pow(z, 2));
        },
        {-100.f,100.f},
        DEFAULT_DRAW_SETTINGS | LIGHTING_ON,
        DEFAULT_COLOR,
        DEFAULT_HEIGHT,
        0.1f
    );
}
