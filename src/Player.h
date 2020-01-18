#include <djf-3d-2/djf-3d.h>
#include "Universe.h"

#ifndef PLAYER_H
#define PLAYER_H

namespace anrchy {

class Player {

private:
    djf_3d::Vec3f position;
    float angular_momentum[3];
    float linear_momentum;
    float angle[3];

public:
    Player(
        const djf_3d::Vec3f& starting_pos
    ) noexcept;

    ~Player(void) noexcept;

    const djf_3d::Vec3f& get_pos(void) const noexcept;

    /**
     * This method is for altering the direction of the
     * player's spacecraft.
     *
     * @param axis the axis about which to pivot
     * @param magnitude the acceleration rate
     */
    template <djf_3d::Axis axis>
    void steer(
        float magnitude
    ) noexcept;

    /**
     * This method is for accelerating the player in
     * whatever direction their ship is pointed.
     *
     * @param magnitude rate of acceleration
     */
    void fire_thrusters(
        float magnitude
    ) noexcept;

    void fire_lasers(void) const noexcept;

    /**
     * This method should be called with each turn of the
     * main game loop.
     *
     * @param uni the Universe in which the player resides
     * @param canvas the canvas upon which things are drawn
     */
    void update_state(
        Universe& uni,
        const djf_3d::Canvas& canvas
    ) noexcept;
};

} // end of namespace anrchy

#endif // PLAYER_H