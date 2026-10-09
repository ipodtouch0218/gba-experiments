#include "bitmap_test.h"

#include "bn_sp_direct_bitmap_bg_builder.h"
#include "bn_log.h"
#include "bn_random.h"

#define SCR_WIDTH 240
#define SCR_HEIGHT 160

BitmapTest::BitmapTest()
    : bitmap_pointer(bn::sp_direct_bitmap_bg_builder{}.build())
{
    bitmap_painter = bn::make_unique<bn::sp_direct_bitmap_bg_painter>(bitmap_pointer);
    bitmap_painter->fill(bn::color{0, 0, 0});

    bn::random rng{};
    p1 = bn::point{ rng.get_unbiased_int(SCR_WIDTH), rng.get_unbiased_int(SCR_HEIGHT) };
    p2 = bn::point{ rng.get_unbiased_int(SCR_WIDTH), rng.get_unbiased_int(SCR_HEIGHT) };
}

void move_point(bn::point &pos, bn::point &vel) {
    pos += vel;

    if (pos.x() < 0 || pos.x() >= SCR_WIDTH) {
        vel.set_x(-vel.x());
    }
    if (pos.y() < 0 || pos.y() >= SCR_HEIGHT) {
        vel.set_y(-vel.y());
    }
}

void BitmapTest::update() {
    move_point(p1, p1_vel);
    move_point(p2, p2_vel);

    bitmap_painter->line(p1_history[history_index], p2_history[history_index], bn::color{ 0, 0, 0 });
    p1_history[history_index] = p1;
    p2_history[history_index] = p2;
    if (++history_index >= p1_history.size()) {
        history_index = 0;
    }

    for (int x = 0; x < p1_history.size(); x++) {
        int index = (x + 9 - history_index) % p1_history.size();
        int col = index * 32 / p1_history.size();
        bitmap_painter->line(p1_history[index], p2_history[index], bn::color{ col, col, col });
    }
}