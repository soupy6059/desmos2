#pragma once

typedef __builtin_va_list __gnuc_va_list;

#include <string>
#include <functional>
#include <complex>

#include "craylib.h"

using Bound = std::pair<float,float>;

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


void draw_complex_function(
    std::function<std::complex<float>(std::complex<float>)> func,
    const Bound &input_bounds,
    unsigned char DRAW_SETTINGS = DEFAULT_DRAW_SETTINGS,
    decltype(DEFAULT_COLOR) &color_func = DEFAULT_COLOR,
    decltype(DEFAULT_HEIGHT) &height_func = DEFAULT_HEIGHT,
    const float &eps = DEFAULT_EPSILON,
    [[maybe_unused]] bool dummy = false
);
