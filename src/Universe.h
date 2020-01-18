#include <djf-3d-2/djf-3d.h>
#include <vector>
#include <memory>
#include <string>

#ifndef UNIVERSE_H
#define UNIVERSE_H

namespace anrchy {

/* The Universe class is a sphere of randomly distributed
   points. Everything in the game happens inside the
   Universe. */
class Universe {

private:
    std::vector<std::unique_ptr<djf_3d::Vec3f>> stars;
    std::vector<std::unique_ptr<djf_3d::Model3d>> asteroids;
    long universe_radius;
    djf_3d::Vec3f center;
    djf_3d::Color star_color;
    djf_3d::Color asteroid_color;

public:
    Universe(
        const djf_3d::Canvas& canvas,
        const long radius,
        const long number_of_stars,
        const djf_3d::Color& init_star_color,
        const std::string& asteroid_path,
        const int number_of_asteroids,
        const djf_3d::Color& init_asteroid_color
    );

    ~Universe(void) noexcept;

    /**
     * This method is for rendering the Universe.
     *
     * @param canvas the Canvas upon which to render
     * @param persp the Perspective to render by
     */
    void render_self(
        djf_3d::Canvas& canvas,
        const djf_3d::Perspective& persp
    ) const noexcept;

    long get_star_count(void) const noexcept;

    const djf_3d::Color& get_star_color(void) const noexcept;

    void set_star_color(const djf_3d::Color& color) noexcept;

    long get_radius(void) const noexcept;

    const djf_3d::Vec3f& get_center(void) const noexcept;

    void set_center(
        const float x,
        const float y,
        const float z
    ) noexcept;

    template <djf_3d::Axis axis>
    void translate(
        const float distance
    ) noexcept;

    template <djf_3d::Axis axis>
    void rotate(
        const float theta_degrees,
        const djf_3d::Canvas& canvas
    ) noexcept;
};

} // end of namespace anrchy

#endif // UNIVERSE_H