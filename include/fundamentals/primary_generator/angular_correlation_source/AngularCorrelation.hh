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
#include <memory>
#include <utility>
#include <vector>

#include "EulerAngleRotation.hh"
#include "State.hh"
#include "Transition.hh"
#include "W_gamma_gamma.hh"

using std::array;
using std::pair;
using std::unique_ptr;
using std::vector;

class AngularCorrelation{

public:
    AngularCorrelation(const State ini_sta, const vector<pair<Transition, State>> st);
    ~AngularCorrelation() = default;

    double operator()(const double theta, const double phi) const;
    double operator()(const double theta, const double phi, const array<double, 3> euler_angles) const;

protected:
    const EulerAngleRotation euler_angle_rotation;
    unique_ptr<W_gamma_gamma> w_gamma_gamma;
};