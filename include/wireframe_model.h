#pragma once

#include "bn_span.h"
#include "bn_fixed.h"
#include "bn_sp_direct_bitmap_bg_painter.h"
#include "bn_color.h"
#include "bn_log.h"

#include "fixed_matrix.h"

class wireframe_line {
    bn::span<const point> points;
    bn::color color{ 31, 31, 31 };
    bool loop{};
    
public:
    void draw(bn::sp_direct_bitmap_bg_painter &painter, const transformation_matrix &matrix) const {
        bn::point previous = project_to_screen_point(points.at(0), matrix);
        bn::point start = previous;
        for (int i = 1; i < points.size(); i++) {
            BN_LOG(previous.x(), " , ", previous.y());
            bn::point next = project_to_screen_point(points.at(i), matrix);
            painter.line(previous, next, color);
            previous = next;
        }
        if (loop) {
            BN_LOG("loop");
            painter.line(previous, start, color);
        }
    }

    constexpr wireframe_line(const bn::span<const point> _points, const bn::color _color, const bool _loop)
        : points(_points),
        color(_color),
        loop(_loop)
    {

    }
};

class wireframe_model {
private:
    bn::span<const wireframe_line> lines;
    
public:
    void draw(bn::sp_direct_bitmap_bg_painter &painter, const transformation_matrix &matrix) const {
        for (auto line : lines) {
            line.draw(painter, matrix);
        }
    }

    constexpr wireframe_model(const bn::span<const wireframe_line> &_lines) : lines(_lines) {}
};