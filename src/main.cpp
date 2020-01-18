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
    djf_3d::Color black = {0, 0, 0, 0};
    anrchy::Universe universe(
        canvas,
        10000,
        10000,
        star_color
    );

    while (!canvas.exit()) {
        canvas.set_draw_color(black);
        canvas.fill_window();
        universe.render_self(canvas, persp);
        universe.rotate<djf_3d::Axis::Z>(0.02);
        canvas.refresh();
    }

    return 0;
}