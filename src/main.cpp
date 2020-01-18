/*          main.cpp file of space-anarchy/
    Copyright (C) 2019  Dante Falzone

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <djf-3d-2/djf-3d.h>
#include <string>
#include "Universe.h"
#include "Player.h"

int main(void) {
    std::string title("Space Anarchy - by Dante Falzone");
    djf_3d::Canvas canvas(
        title,
        800,
        600,
        -1100
    );

    djf_3d::Perspective persp(400, 300, 1200.0);
    djf_3d::Color star_color = {255, 255, 200, 0};
    djf_3d::Color asteroid_color = {255, 50, 0, 0};
    djf_3d::Color green = {0, 255, 50, 0};
    djf_3d::Color black = {0, 0, 0, 0};

    anrchy::Universe universe(
        canvas,
        10000,
        10000,
        star_color,
        "assets/asteroid.obj",
        20,
        asteroid_color
    );

    anrchy::Player player(universe.get_center());

    djf_3d::KeyboardState keyboard_state;

    while (!canvas.exit()) {
        keyboard_state = canvas.get_keyboard_state();
        canvas.set_draw_color(black);
        canvas.fill_window();
        universe.render_self(canvas, persp);

        // draw the crosshairs
        canvas.set_draw_color(green);
        canvas.draw_line(390, 290, 410, 310);
        canvas.draw_line(390, 310, 410, 290);

        if (keyboard_state.W)
            player.steer<djf_3d::Axis::X>(+0.001);
        if (keyboard_state.S)
            player.steer<djf_3d::Axis::X>(-0.001);
        if (keyboard_state.A)
            player.steer<djf_3d::Axis::Z>(+0.001);
        if (keyboard_state.D)
            player.steer<djf_3d::Axis::Z>(-0.001);
        if (keyboard_state.Q)
            player.steer<djf_3d::Axis::Y>(+0.001);
        if (keyboard_state.E)
            player.steer<djf_3d::Axis::Y>(-0.001);
        if (keyboard_state.up_arr)
            player.fire_thrusters(-0.01);
        if (keyboard_state.down_arr)
            player.fire_thrusters(+0.01);

        canvas.refresh();
        player.update_state(universe, canvas);
    }

    return 0;
}