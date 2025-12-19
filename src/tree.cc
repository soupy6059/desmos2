#include <iostream>
#include <variant>
#include <string>
#include <memory>

namespace EXPR {
    struct VAR;
    struct APPL;
    struct FUNC;
    using T = std::variant<EXPR::FUNC,EXPR::VAR,EXPR::APPL>;
};

struct EXPR::VAR {
    std::string name;
    std::size_t id = 0;
    bool is_free() { return id == 0; }
    static std::unique_ptr<EXPR::VAR> make(std::string name, std::size_t id) {
        return std::make_unique<EXPR::VAR>(name,id);
    }
};

struct EXPR::APPL {
    std::unique_ptr<EXPR::T> left;
    std::unique_ptr<EXPR::T> right;
    static std::unique_ptr<EXPR::APPL> make(decltype(left) left, decltype(right) right) {
        return std::make_unique<EXPR::APPL>(std::move(left),std::move(right));
    }
};

struct EXPR::FUNC {
    EXPR::VAR arg;
    std::unique_ptr<EXPR::T> body;
    static std::unique_ptr<EXPR::APPL> make(decltype(left) left, decltype(right) right) {
        return std::make_unique<EXPR::APPL>(std::move(left),std::move(right));
    }
};
