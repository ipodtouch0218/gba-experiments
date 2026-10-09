#pragma once

#include "bn_sp_direct_bitmap_bg_ptr.h"
#include "bn_sp_direct_bitmap_bg_painter.h"
#include "bn_point.h"
#include "bn_array.h"
#include "bn_unique_ptr.h"

#define POINT_HISTORY 8

class BitmapTest {
private:
    bn::sp_direct_bitmap_bg_ptr bitmap_pointer;
    bn::unique_ptr<bn::sp_direct_bitmap_bg_painter> bitmap_painter;
    bn::point p1;
    bn::point p1_vel{3, -3};
    bn::point p2;
    bn::point p2_vel{3, -3};
    int history_index{};
    bn::array<bn::point, POINT_HISTORY> p1_history;
    bn::array<bn::point, POINT_HISTORY> p2_history;
public:
    BitmapTest();
    void update();
};