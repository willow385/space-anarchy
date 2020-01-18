#include <djf-3d-2/djf-3d.h>
#include <vector>
#include <memory>
#include <random>
#include "Universe.h"

namespace anrchy {

Universe::Universe(
     const djf_3d::Canvas& canvas,
     const long radius,
     const long number_of_stars,
     const djf_3d::Color& init_star_color
): center(
    canvas.get_width() / 2,
    canvas.get_height() / 2,
    canvas.get_viewer_y_pos() / 2
) {
    universe_radius = radius;
    star_color.red = init_star_color.red;
    star_color.green = init_star_color.green;
    star_color.blue = init_star_color.blue;
    star_color.alpha = init_star_color.alpha;

    std::random_device rand_dev;
    std::mt19937 rand_gen(rand_dev());
    std::uniform_real_distribution<> rand_angle(0.0, 360.0);
    for (long i = 0; i < number_of_stars; i++) {
        std::unique_ptr<djf_3d::Vec3f> star(
            new djf_3d::Vec3f(
                center.get_pos<djf_3d::Axis::X>(),
                center.get_pos<djf_3d::Axis::Y>(),
                universe_radius
            )
        );
        float x_angle, y_angle, z_angle;
        x_angle = rand_angle(rand_gen);
        y_angle = rand_angle(rand_gen);
        z_angle = rand_angle(rand_gen);
        star->rotate_3d<djf_3d::Axis::X>(center, x_angle);
        star->rotate_3d<djf_3d::Axis::Y>(center, y_angle);
        star->rotate_3d<djf_3d::Axis::Z>(center, z_angle);
        stars.push_back(std::move(star));
    }
}

Universe::~Universe(void) noexcept {}

void Universe::render_self(
    djf_3d::Canvas& canvas,
    djf_3d::Perspective& persp
) const noexcept {
    for (auto& star: stars) {
        canvas.draw_point(
            *star,
            persp
        );
    }
}

long Universe::get_star_count(void) const noexcept {
    return stars.size();
}

const djf_3d::Color& Universe::get_star_color(void)
const noexcept {
    return star_color;
}

void Universe::set_star_color(
    const djf_3d::Color& color
) noexcept {
    star_color.red = color.red;
    star_color.green = color.green;
    star_color.blue = color.blue;
    star_color.alpha = color.alpha;
}

long Universe::get_radius(void) const noexcept {
    return universe_radius;
}

const djf_3d::Vec3f& Universe::get_center(void)
const noexcept {
    return center;
}

void Universe::set_center(
    const float x,
    const float y,
    const float z
) noexcept {
    center.set_position(x, y, z);
}

template <djf_3d::Axis axis>
void Universe::translate(const float distance) noexcept {
    center.translate<axis>(distance);
    for (auto& star: stars) {
        star->translate<axis>(distance);
    }
}

template void Universe::translate<djf_3d::Axis::X>(
    const float distance
) noexcept;
template void Universe::translate<djf_3d::Axis::Y>(
    const float distance
) noexcept;
template void Universe::translate<djf_3d::Axis::Z>(
    const float distance
) noexcept;


template <djf_3d::Axis axis>
void Universe::rotate(
    const float theta_degrees
) noexcept {
    for (auto& star: stars) {
        star->rotate_3d<axis>(center, theta_degrees);
    }
}

template void Universe::rotate<djf_3d::Axis::X>(
    const float theta_degrees
) noexcept;
template void Universe::rotate<djf_3d::Axis::Y>(
    const float theta_degrees
) noexcept;
template void Universe::rotate<djf_3d::Axis::Z>(
    const float theta_degrees
) noexcept;

} // end of namespace anrchy