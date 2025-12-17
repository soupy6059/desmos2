#pragma once

#include <iostream>
#include <vector>
#include <cassert>
#include <functional>
#include <exception>
#include <array>


template<std::size_t size, typename number_t = float>
class algebraic_vector {
    std::function<number_t&(std::size_t)> accessor;
    static constexpr bool log_it = true;
    static constexpr std::ostream &os = std::cout;
public:
    algebraic_vector(decltype(accessor) accessor): accessor{accessor} {
        if constexpr(log_it) os << "algebraic_vector<" << size << "> basic ctor\n";
    }
    ~algebraic_vector() {
        if constexpr(log_it) os << "algebraic_vector<" << size << "> dtor\n";
    }
    constexpr std::size_t length() const { return size; }
    number_t &operator[](std::size_t index) {
        if(index >= size) throw std::out_of_range("bad index");
        return accessor(index);
    }
    number_t operator*(const algebraic_vector<size> &other) {
        number_t accumulator{0};
        for(std::size_t i{0}; i < size; ++i) {
            accumulator += accessor(i) * other.accessor(i);
        }
        return accumulator;
    }
    std::function<number_t(algebraic_vector<size,number_t>)> dual() {
        return {[this](algebraic_vector<size,number_t> v) -> number_t {
            return (*this) * v;
        }};
    }
};

template<std::size_t rows, std::size_t cols, typename number_t = float>
class matrix {
    std::array<std::array<number_t,cols>,rows> data;
    [[maybe_unused]] constexpr static bool log_it = true;
    [[maybe_unused]] constexpr static std::ostream &os = std::cout;
public:
    class view {
    public:
        using index_t = std::pair<std::size_t,std::size_t>;
    private:
        decltype(data) &source;
        std::vector<std::function<index_t(index_t)>> funcs;
    public:
        view(decltype(data) &source): source{source} {}
        decltype(funcs) &get() { return funcs; }
        number_t &at(index_t index) {
            for(const auto &func: funcs) index = func(std::move(index));
            return source[index.first][index.second];
        }
        void foreach(std::function<void(index_t,number_t&)> func) {
            for(std::size_t i{0}; i < rows; ++i) {
                for(std::size_t j{0}; j < cols; ++j) {
                    func({i,j}, at({i,j}));
                }
            }
        }
        void print(std::ostream &os) {
            for(std::size_t i{0}; i < rows; ++i) {
                os << "[";
                for(std::size_t j{0}; j < cols; ++j) {
                    os << at({i,j});
                    if(j < cols - 1) os << " ";
                }
                os << "]\n";
            }
        }
        algebraic_vector<cols> row_vector(std::size_t i) {
            return {
                [this,i](std::size_t k) -> number_t &{
                    return this->source[i][k];
                }
            };
        }
        algebraic_vector<rows> col_vector(std::size_t j) {
            return {
                [this,j](std::size_t k) -> number_t &{
                    return this->source[j][k];
                }
            };
        }
        void draw() {
            assert(rows == 3);
            for(std::size_t i{0}; i < cols; ++i) {
                auto r {col_vector(i)};
                rl::Vector3 vec_from_row {r[0], r[1], r[2]};
                rl::DrawLine3D(rl::Vector3Zero(), vec_from_row, rl::BLUE);
            }
        }
    };
    matrix(): data{} {
        if constexpr(log_it) os << "matrix<" << cols << "," << rows << "> basic ctor\n";
    }
    matrix(const matrix<rows,cols,number_t> &other): data{other.data} {
        if constexpr(log_it) os << "matrix<" << cols << "," << rows << "> copy ctor\n";
    }
    matrix(matrix<rows,cols,number_t> &&other): data{std::move(other.data)} {
        if constexpr(log_it) os << "matrix<" << cols << "," << rows << "> move ctor\n";
    }
    ~matrix() {
        if constexpr(log_it) os << "matrix<" << cols << "," << rows << "> dtor\n";
    }

    static matrix<rows,cols,number_t> ones() {
        matrix<rows,cols,number_t> E;
        E.identity().foreach([](auto _[[maybe_unused]], auto &x) { x = 1; });
        return E;
    }
    static matrix<rows,cols,number_t> I() {
        matrix<rows,cols,number_t> id;
        id.identity().foreach([](auto index, auto &x) {
            auto [i,j] = index;
            if(i == j) x = 1;
            else x = 0;
        });
        return id;
    }

    static constexpr std::size_t row_count() { return rows; }
    static constexpr std::size_t column_count() { return cols; }
    std::array<number_t,cols> &operator[](std::size_t index) {
        return data[index];
    }
    view identity() {
        return {data};
    }
    view transpose() {
        view trans{data};
        trans.get().push_back([](view::index_t &&index) -> view::index_t {
            return {index.second, index.first};
        });
        return trans;
    }
    number_t &at(std::size_t i, std::size_t j) {
        return data[i][j];
    }

    algebraic_vector<cols> row_vector(std::size_t i) {
        return {
            [this,i](std::size_t k) -> number_t &{
                return this->data[i][k];
            }
        };
    }
    algebraic_vector<rows> col_vector(std::size_t j) {
        return {
            [this,j](std::size_t k) -> number_t &{
                return this->data[j][k];
            }
        };
    }

    template<std::size_t other_cols>
    matrix<rows,other_cols,number_t> operator*(matrix<cols,other_cols,number_t> &other) {
        matrix<rows,other_cols,number_t> answer{};
        for(std::size_t i{0}; i < rows; ++i)
            for(std::size_t j{0}; j < other_cols; ++j)
                answer.at(i,j) = row_vector(i) * other.col_vector(j);
        return answer;
    }
    template<std::size_t other_cols>
    matrix<rows,other_cols,number_t> operator*(matrix<cols,other_cols,number_t> &&other) {
        matrix<rows,other_cols,number_t> answer{};
        for(std::size_t i{0}; i < rows; ++i)
            for(std::size_t j{0}; j < other_cols; ++j)
                answer.at(i,j) = row_vector(i) * other.col_vector(j);
        return answer;
    }
};

