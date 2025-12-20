#pragma once

typedef __builtin_va_list __gnuc_va_list;

#include <string>
#include <functional>
#include <complex>

#include "craylib.h"

namespace dcf {

using R = float;
using Bound = std::pair<R,R>;
using C = std::complex<R>;
using C2 = std::pair<C,C>;
using C_to_C = std::function<C(C)>; 
using C_to_C2 = std::function<C2(C)>;
using C_to_R = std::function<R(C)>;
using C_to_Color = std::function<rl::Color(C)>;

// SETTINGS
constexpr static const unsigned char OUTPUT = 0b00000001;
constexpr static const unsigned char RADIAL_OUT = 0b00000001;
constexpr static const unsigned char CARTESION_OUT = 0b00000000;
constexpr static const unsigned char INPUT = 0b00000010;
constexpr static const unsigned char RADIAL_IN = 0b00000010;
constexpr static const unsigned char CARTESION_IN = 0b00000000;
constexpr static const unsigned char LIGHTING = 0b00000100;
constexpr static const unsigned char LIGHTING_ON = 0b00000100;
constexpr static const unsigned char LIGHTING_OFF = 0b00000000;

// DEFAULTS
constexpr static const float DEFAULT_EPSILON = 0.1f;
constexpr static const unsigned char DEFAULT_DRAW_SETTINGS = CARTESION_IN | RADIAL_OUT;

static const std::function<rl::Color(std::complex<float>)> DEFAULT_COLOR =
[](std::complex<float> z) -> rl::Color {
    return rl::ColorFromHSV(std::arg(z) / std::acos(-1) * 180.f + 180.f, 1.f, 0.75f);
};

static const std::function<float(std::complex<float>)> DEFAULT_HEIGHT =
[](std::complex<float> z) -> float {
    return std::abs(z);
};

static decltype(DEFAULT_HEIGHT) NEGATIVE_DEFAULT_HEIGHT =
[](std::complex<float> z) -> float {
    return -DEFAULT_HEIGHT(z);
};


struct settings {
    enum class LIGHTING {
        ON, OFF
    };
    Bound input_bounds;
    R epsilon;
    C_to_R height;
    C_to_Color color;
    LIGHTING lighting;
};

namespace defns {
    static const C_to_Color phase_to_hue {[](C z) -> rl::Color {
        return rl::ColorFromHSV(std::arg(z) / std::acos(-1) * 180.f + 180.f, 1.f, 0.75f);
    }};
};

namespace defaults {
    static const settings::LIGHTING lighting = settings::LIGHTING::ON;
    static const Bound bound {-5.0f,5.0f};
    static const R epsilon {0.1f};
    static const C_to_R height = [](C z) -> R { return std::abs(z); };
    static const C_to_Color color {defns::phase_to_hue};
    settings make();
};

void draw_complex_function(
    C_to_C func,
    settings setting
);

void draw_complex_function(
    C_to_C2 func,
    settings setting
);

};
