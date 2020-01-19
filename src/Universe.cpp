#include <djf-3d-2/djf-3d.h>
#include <vector>
#include <string>
#include <memory>
#include <random>
#include "Universe.h"

namespace anrchy {

Universe::Universe(
    const djf_3d::Canvas& canvas,
    const long radius,
    const long number_of_stars,
    const djf_3d::Color& init_star_color,
    const std::string& asteroid_path,
    const int number_of_asteroids,
    const djf_3d::Color& init_asteroid_color,
    const std::string& planet_path,
    const djf_3d::Color& init_planet_color
): center(
    canvas.get_width() / 2,
    canvas.get_height() / 2,
    canvas.get_viewer_y_pos() / 2
), planet(planet_path) {
    universe_radius = radius;
    star_color.red = init_star_color.red;
    star_color.green = init_star_color.green;
    star_color.blue = init_star_color.blue;
    star_color.alpha = init_star_color.alpha;
    asteroid_color.red = init_asteroid_color.red;
    asteroid_color.green = init_asteroid_color.green;
    asteroid_color.blue = init_asteroid_color.blue;
    asteroid_color.alpha = init_asteroid_color.alpha;
    planet_color.red = init_planet_color.red;
    planet_color.green = init_planet_color.green;
    planet_color.blue = init_planet_color.blue;
    planet_color.alpha = init_planet_color.alpha;

    std::random_device rand_dev;
    std::mt19937 rand_gen(rand_dev());
    std::uniform_real_distribution<> rand_angle(0.0, 360.0);
    std::uniform_real_distribution<> rand_asteroid_pos(
        static_cast<float>(-1 * universe_radius) / 30,
        static_cast<float>(universe_radius) / 30
    );
    std::uniform_real_distribution<> rand_planet_pos(
        static_cast<float>(universe_radius * 50),
        static_cast<float>(universe_radius * 100)
    );

    planet.set_position(
        djf_3d::Vec3f(
            rand_planet_pos(rand_gen),
            rand_planet_pos(rand_gen),
            rand_planet_pos(rand_gen)
        )
    );
    planet.rotate_self<djf_3d::Axis::X>(
        rand_angle(rand_gen)
    );
    planet.rotate_self<djf_3d::Axis::Y>(
        rand_angle(rand_gen)
    );
    planet.rotate_self<djf_3d::Axis::Z>(
        rand_angle(rand_gen)
    );

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

    for (int i = 0; i < number_of_asteroids; i++) {
        std::unique_ptr<djf_3d::Model3d> asteroid(
            new djf_3d::Model3d(asteroid_path)
        );
        djf_3d::Vec3f new_pos(
            rand_asteroid_pos(rand_gen),
            rand_asteroid_pos(rand_gen),
            rand_asteroid_pos(rand_gen)
        );
        asteroid->set_position(new_pos);
        asteroids.push_back(std::move(asteroid));
    }
}

/* No need to do any work here -- smart pointers will handle all
   the heap-allocated memory for us */
Universe::~Universe(void) noexcept {}

void Universe::render_self(
    djf_3d::Canvas& canvas,
    const djf_3d::Perspective& persp
) const noexcept {
    canvas.set_draw_color(star_color);
    for (auto& star: stars) {
        canvas.draw_point(
            *star,
            persp
        );
    }
    canvas.set_draw_color(asteroid_color);
    for (auto& asteroid: asteroids) {
        canvas.draw_model3d(
            *asteroid,
            persp
        );
    }

    canvas.set_draw_color(planet_color);
    canvas.draw_model3d(planet, persp);
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
    for (auto& asteroid: asteroids) {
        asteroid->translate<axis>(distance);
    }
    planet.translate<axis>(distance);
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
    const float theta_degrees,
    const djf_3d::Canvas& canvas
) noexcept {
    for (auto& star: stars) {
        star->rotate_3d<axis>(center, theta_degrees);
    }

    djf_3d::Vec3f player_pos(
        canvas.get_width() / 2,
        canvas.get_viewer_y_pos(),
        canvas.get_height() / 2
    );

    for (auto& asteroid: asteroids) {
        asteroid->rotate_around<axis>(
            player_pos, theta_degrees
        );
    }

    planet.rotate_around<axis>(player_pos, theta_degrees);
}

template void Universe::rotate<djf_3d::Axis::X>(
    const float theta_degrees,
    const djf_3d::Canvas& canvas
) noexcept;
template void Universe::rotate<djf_3d::Axis::Y>(
    const float theta_degrees,
    const djf_3d::Canvas& canvas
) noexcept;
template void Universe::rotate<djf_3d::Axis::Z>(
    const float theta_degrees,
    const djf_3d::Canvas& canvas
) noexcept;

} // end of namespace anrchy
