#include <djf-3d-2/djf-3d.h>
#include <cmath>
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
    is_dead = false;
    fuel = 100.0;

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
    if (fuel <= 0) { fuel = 0; return; }
    angular_momentum[static_cast<int>(axis)] += magnitude;
    fuel -= 0.025;
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
    if (fuel <= 0) { fuel = 0; return; }
    linear_momentum += magnitude;
    fuel -= 0.025;
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
            fuel += 2;
            if (fuel > 100) fuel = 100.0;
        }
    }
}

float Player::get_linear_momentum(void) const noexcept {
    return linear_momentum;
}

void Player::update_state(
    Universe& uni,
    const djf_3d::Perspective& persp,
    const djf_3d::Canvas& canvas
) noexcept {
    if (fuel <= 0) {is_dead = true; return; }

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

    /* Scan all the asteroids in the universe to see if
       we're colliding with any of them */
    int asteroid_cnt = uni.get_asteroid_cnt();
    for (int i = 0; i < asteroid_cnt; i++) {
        float x, y, z;
        x = uni[i].nth_vertex(0)
            .project_2d_x(persp);
        y = uni[i].nth_vertex(0)
            .get_pos<djf_3d::Axis::Y>();
        z = uni[i].nth_vertex(0)
            .project_2d_y(persp);

        int y_upper_lim = canvas.get_viewer_y_pos() + 10;
        int y_lower_lim = y_upper_lim - 20;

        // Die if we're colliding with an asteroid
        if (
            x > 300 && x < 500
        &&
            y < y_upper_lim && y > y_lower_lim
        &&
            z > 200 && z < 400

        ) {
            is_dead = true;
        }
    }
}

} // end of namespace anrchy