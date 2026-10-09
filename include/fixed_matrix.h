#pragma once

#include "bn_fixed.h"
#include "bn_array.h"
#include "bn_math.h"
#include "bn_string.h"
#include "bn_point.h"

template<int Rows, int Columns>
class fixed_matrix {
    bn::array<bn::fixed, Rows * Columns> matrix;
public:
    constexpr const bn::fixed& at(int index) const { return matrix.at(index); }
    constexpr const bn::fixed& at(int x, int y) const { return at(x * Columns + y); }
    constexpr bn::fixed& at(int index) { return matrix.at(index); }
    constexpr bn::fixed& at(int x, int y) { return at(x * Columns + y); }
    constexpr int rows() const { return Rows; }
    constexpr int columns() const { return Columns; }
    constexpr int size() const { return Rows * Columns; }

    template<int RightColumns>
    constexpr fixed_matrix<Rows, RightColumns> multiply(const fixed_matrix<Columns, RightColumns> &right) const {
        fixed_matrix<Rows, RightColumns> result;

        for (int r_index = 0; r_index < right.size(); r_index++) {
            bn::fixed &ref = result.at(r_index);
            int r_column = r_index % right.columns();
            int l_row = r_index / right.columns();

            for (int x = 0; x < Columns; x++) {
                ref += at(l_row, x) * right.at(x, r_column);
            }
        }
        
        return result;
    }

    constexpr bn::string<256> to_string() const {
        bn::string<256> result = "[";
        for (int row = 0; row < Rows; row++) {
            for (int column = 0; column < Columns; column++) {
                result += bn::to_string<16>(at(row, column));
                if (column != Columns - 1) {
                    result += ",";
                }
            }
            if (row != Rows - 1) {
                result += "\n";
            }
        }
        result += "]";
        return result;
    }

    constexpr fixed_matrix() = default;
    constexpr fixed_matrix(const bn::array<bn::fixed, Rows * Columns> &other) { matrix = other; }
    static_assert(Rows > 0);
    static_assert(Columns > 0);
};

using transformation_matrix = fixed_matrix<4,4>;
using point = fixed_matrix<4,1>;

constexpr transformation_matrix IDENTITY = {bn::array<bn::fixed, 16>{
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
}};

transformation_matrix translate(bn::fixed x, bn::fixed y, bn::fixed z) {
    return {bn::array<bn::fixed, 16>{
        1, 0, 0, x,
        0, 1, 0, y,
        0, 0, 1, z,
        0, 0, 0, 1
    }};
}

transformation_matrix scale(bn::fixed x, bn::fixed y, bn::fixed z) {
    return {bn::array<bn::fixed, 16>{
        x, 0, 0, 0,
        0, y, 0, 0,
        0, 0, z, 0,
        0, 0, 0, 1
    }};
}

transformation_matrix rotate_x(bn::fixed x) {
    return {bn::array<bn::fixed, 16>{
        1, 0, 0, 0,
        0, bn::cos(x), -bn::sin(x), 0,
        0, bn::sin(x), bn::cos(x), 0,
        0, 0, 0, 1
    }};
}

transformation_matrix rotate_y(bn::fixed y) {
    return {bn::array<bn::fixed, 16>{
        bn::cos(y), 0, bn::sin(y), 0,
        0, 1, 0, 0,
        -bn::sin(y), 0, bn::cos(y), 0,
        0, 0, 0, 1
    }};
}

transformation_matrix rotate_z(bn::fixed z) {
    return {bn::array<bn::fixed, 16>{
        bn::cos(z), -bn::sin(z), 0, 0,
        bn::sin(z), bn::cos(z), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    }};
}

bn::point project_to_screen_point(const point &x, const transformation_matrix &matrix) {
    point projected = matrix.multiply(x);
    return bn::point{ projected.at(0).round_integer(), -projected.at(1).round_integer() };
}