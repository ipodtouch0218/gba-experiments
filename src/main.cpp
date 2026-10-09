#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_log.h"
#include "bn_sp_direct_bitmap_bg_ptr.h"
#include "bn_sp_direct_bitmap_bg_builder.h"
#include "bn_sp_direct_bitmap_bg_painter.h"
#include "bn_color.h"
#include "bn_math.h"

#include "fixed_matrix.h"
#include "wireframe_model.h"

constexpr bn::array<const point, 4> cube_points_1{
    point{bn::array<bn::fixed, 4>{-1, -1, -1, 1}},
    point{bn::array<bn::fixed, 4>{ 1, -1, -1, 1}},
    point{bn::array<bn::fixed, 4>{ 1,  1, -1, 1}},
    point{bn::array<bn::fixed, 4>{-1,  1, -1, 1}}
};

constexpr bn::array<const point, 4> cube_points_2{
    point{bn::array<bn::fixed, 4>{-1, -1,  1, 1}},
    point{bn::array<bn::fixed, 4>{ 1, -1,  1, 1}},
    point{bn::array<bn::fixed, 4>{ 1,  1,  1, 1}},
    point{bn::array<bn::fixed, 4>{-1,  1,  1, 1}}
};
    
constexpr bn::array<const point, 2> cube_points_3{
    point{bn::array<bn::fixed, 4>{-1, -1, -1, 1}},
    point{bn::array<bn::fixed, 4>{-1, -1,  1, 1}}
};
    
constexpr bn::array<const point, 2> cube_points_4{
    point{bn::array<bn::fixed, 4>{ 1, -1, -1, 1}},
    point{bn::array<bn::fixed, 4>{ 1, -1,  1, 1}}
};
    
constexpr bn::array<const point, 2> cube_points_5{
    point{bn::array<bn::fixed, 4>{ 1,  1, -1, 1}},
    point{bn::array<bn::fixed, 4>{ 1,  1,  1, 1}}
};
    
constexpr bn::array<const point, 2> cube_points_6{
    point{bn::array<bn::fixed, 4>{-1,  1, -1, 1}},
    point{bn::array<bn::fixed, 4>{-1,  1,  1, 1}}
};

constexpr bn::span<const point> cube_points_span_1{ cube_points_1 };
constexpr bn::span<const point> cube_points_span_2{ cube_points_2 };
constexpr bn::span<const point> cube_points_span_3{ cube_points_3 };
constexpr bn::span<const point> cube_points_span_4{ cube_points_4 };
constexpr bn::span<const point> cube_points_span_5{ cube_points_5 };
constexpr bn::span<const point> cube_points_span_6{ cube_points_6 };

constexpr bn::array<const wireframe_line, 6> cube_wireframe_lines{
    wireframe_line{ cube_points_span_1, bn::color{31, 31, 31}, true },
    wireframe_line{ cube_points_span_2, bn::color{31, 31, 31}, true },
    wireframe_line{ cube_points_span_3, bn::color{31, 31, 31}, false },
    wireframe_line{ cube_points_span_4, bn::color{31, 31, 31}, false },
    wireframe_line{ cube_points_span_5, bn::color{31, 31, 31}, false },
    wireframe_line{ cube_points_span_6, bn::color{31, 31, 31}, false }
};
constexpr bn::span<const wireframe_line> cube_wireframe_lines_span{ cube_wireframe_lines };

constexpr wireframe_model cube{ cube_wireframe_lines_span };


int main() {
    bn::core::init();
    
    bn::sp_direct_bitmap_bg_ptr bitmap_pointer = bn::sp_direct_bitmap_bg_builder{}.build();
    bn::sp_direct_bitmap_bg_painter bitmap_painter{bitmap_pointer};
    
    transformation_matrix model_matrix = IDENTITY;
    bn::fixed pos_x{}, pos_y{}, pos_z{};
    bn::fixed euler_x{}, euler_y{}, euler_z{};

    while(true) {

        bn::core::update();

        if (bn::keypad::l_held()) {
            if (bn::keypad::left_held()) {
                euler_y += 0.01;
            }
            if (bn::keypad::right_held()) {
                euler_y += -0.01;
            }
            if (bn::keypad::up_held()) {
                euler_x += 0.01;
            }
            if (bn::keypad::down_held()) {
                euler_x += -0.01;
            }
        } else {
            if (bn::keypad::left_held()) {
                pos_x += -1;
            }
            if (bn::keypad::right_held()) {
                pos_x += 1;
            }
            if (bn::keypad::up_held()) {
                pos_y += 1;
            }
            if (bn::keypad::down_held()) {
                pos_y += -1;
            }
        }

        model_matrix = IDENTITY
            .multiply(translate(pos_x, pos_y, pos_z))
            .multiply(rotate_x(euler_x))
            .multiply(rotate_z(euler_y))
            .multiply(rotate_y(euler_z))
            .multiply(scale(30, 30, 30));

        bitmap_painter.fill(bn::color{ 0, 0, 0 });
        cube.draw(bitmap_painter, model_matrix);
    }
}