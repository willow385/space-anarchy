#include <djf-3d-2/djf-3d.h>
#include "Universe.h"
#include "Player.h"

namespace anrchy {

Player::Player(const djf_3d::Vec3f& starting_pos)
noexcept: position(
    starting_pos.get_pos<djf_3d::Axis::X>(),
    starting_pos.get_pos<djf_3d::Axis::Y>(),
    starting_pos.get_pos<djf_3d::Axis::Z>()
) {
    score = 0;

    for (int i = 0; i < 3; i++) {
        angular_momentum[i] = 0.0;
    }
    linear_momentum = 0.0;
}

Player::~Player(void) noexcept {}

const djf_3d::Vec3f& Player::get_pos(void) const noexcept {
    return position;
}

template <djf_3d::Axis axis>
void Player::steer(float magnitude) noexcept {
    angular_momentum[static_cast<int>(axis)] += magnitude;
}

template void Player::steer<djf_3d::Axis::X>(
    float magnitude
) noexcept;
template void Player::steer<djf_3d::Axis::Y>(
    float magnitude
) noexcept;
template void Player::steer<djf_3d::Axis::Z>(
    float magnitude
) noexcept;

void Player::fire_thrusters(float magnitude) noexcept {
    linear_momentum += magnitude;
}

void Player::fire_lasers(
    djf_3d::Canvas& canvas,
    const djf_3d::Perspective& persp,
    Universe& uni
) noexcept {
    djf_3d::Color laser_red = {255, 20, 20, 0};
    canvas.set_draw_color(laser_red);
    for (int y = 290; y <= 310; y++) {
        canvas.draw_line(0, y, 400, 300);
        canvas.draw_line(800, y, 400, 300);
    }

    int asteroid_cnt = uni.get_asteroid_cnt();
    for (int i = 0; i < asteroid_cnt; i++) {
        float x =
            uni[i]
            .nth_vertex(0)
            .project_2d_x(persp);
        float y =
            uni[i]
            .nth_vertex(0)
            .project_2d_y(persp);

        if (x > 380 && x < 420 && y > 280 && y < 320) {
            uni.destroy_asteroid(i);
            score++;
        }
    }
}

float Player::get_linear_momentum(void) const noexcept {
    return linear_momentum;
}

void Player::update_state(
    Universe& uni,
    const djf_3d::Canvas& canvas
) noexcept {
    uni.rotate<djf_3d::Axis::X>(
        angular_momentum[static_cast<int>(djf_3d::Axis::X)],
        canvas
    );
    uni.rotate<djf_3d::Axis::Y>(
        angular_momentum[static_cast<int>(djf_3d::Axis::Y)],
        canvas
    );
    uni.rotate<djf_3d::Axis::Z>(
        angular_momentum[static_cast<int>(djf_3d::Axis::Z)],
        canvas
    );

    uni.translate<djf_3d::Axis::Y>(
        linear_momentum
    );

    for (int i = 0; i < 3; i++) {
        angle[i] += angular_momentum[i];
    }
}

} // end of namespace anrchy