/*
    This file is part of nutr.

    nutr is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    nutr is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with nutr.  If not, see <https://www.gnu.org/licenses/>.

    Copyright (C) 2020 Udo Friman-Gayer
*/

#pragma once

#include <array>
#include <utility>
#include <vector>

#include "State.hh"
#include "Transition.hh"

using std::array;
using std::pair;
using std::vector;

// Weisstein (Wolfram MathWorld) probably best reference for Euler angles.
// https://mathworld.wolfram.com/EulerAngles.html
class W_gamma_gamma{
    W_gamma_gamma(const State ini_sta, const vector<pair<Transition, State>> st);
    ~W_gamma_gamma() = default;

    double operator()(const double theta, const double phi, const array<double, 3> euler_angles = {0., 0., 0.}) const;
};