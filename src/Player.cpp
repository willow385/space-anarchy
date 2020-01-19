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

void Player::fire_lasers(void) const noexcept {
    // TODO: implement
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