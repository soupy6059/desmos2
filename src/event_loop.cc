#include "event_loop.h"
#include "craylib.h"
#include "draw_complex_function.h"

#include <complex>
#include <map>
#include <numeric>

constexpr static inline auto unity = [](int n) constexpr -> dcf::C {
    using namespace std::complex_literals;
    return std::exp(2.if * std::acos(-1.f) / dcf::C{static_cast<float>(n)});
};
constexpr static inline auto cyclotonic = [](int n) constexpr -> dcf::C_to_C2 {
    using namespace dcf;
    using namespace std::complex_literals;
    return [n](C z) constexpr -> C2 {
        C w = 1.f;

        for(int k = 1; k <= n; ++k) {
            if(std::gcd(k, n) == 1) 
                w *= z - std::pow(unity(n), static_cast<C>(k));
        }
    
        return {z, w/(std::abs(w)+1)};
    };
};

constexpr static inline auto mobius = [](dcf::C a, dcf::C b, dcf::C c, dcf::C d) constexpr -> dcf::C_to_C2 {
    using namespace dcf;
    using namespace std::complex_literals;
    return [a,b,c,d](C z) constexpr -> C2 {
        C w = (a*z + b)/(c*z + d);
        return {z, w/(std::abs(w)+1)};
    };
};

constexpr static inline auto mobius_inv = [](dcf::C a, dcf::C b, dcf::C c, dcf::C d) constexpr -> dcf::C_to_C2 {
    return mobius(d, -b, -c, a);
};

void event_loop(float t, float t2, rl::Vector2 cord, float eps, float bound_radius) {
    using namespace std::complex_literals;
    using namespace dcf;

    static C_to_C f {
    [&t](C z) -> C {
        return std::atan(z);
    }};

    // Mobius s/t 0  |-> 1
    // Mobius s/t 1  |-> oo
    // Mobius s/t oo |-> 0
    //
    // (-1) 1 / x - 1

    dcf::settings s {defaults::make()};
    s.input_bounds = Bound{-bound_radius,bound_radius};
    s.epsilon = eps;
    static C_to_C2 g = [&t,&cord,&t2](C z) -> C2 {
        C mu = 0.5;
        C w = std::pow(z,C{3.0}) + mu * z;
        return { z, w/(std::abs(w)+C{1.0}) };
    };

    dcf::draw_complex_function(
        [&](C z) -> C2 {
            C w = z*z + z + C{2};
            return {  z, w / (std::abs(w)+ C{1.0}) };
        } 
    , s);

//    dcf::draw_complex_function(
//        mobius(C{1.0}, C{0.0}, C{0.0}, C{1.0}), s
//    );

    //dcf::draw_complex_function(
        //f, defaults::make()
    //);
}
