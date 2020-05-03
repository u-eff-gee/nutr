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

#include "AngularCorrelation.hh"
#include "W_dir_dir.hh"
#include "W_pol_dir.hh"

AngularCorrelation::AngularCorrelation(const State ini_sta, const vector<pair<Transition, State>> cas_ste):
    euler_angle_rotation(EulerAngleRotation()), w_gamma_gamma(nullptr)
{
    if(cas_ste[0].first.em_char == em_unknown){
        w_gamma_gamma = std::make_unique<W_dir_dir>(ini_sta, cas_ste);
    } else{
        w_gamma_gamma = std::make_unique<W_pol_dir>(ini_sta, cas_ste);
    }
}

double AngularCorrelation::operator()(const double theta, const double phi) const {
    return w_gamma_gamma->operator()(theta, phi);
}

double AngularCorrelation::operator()(const double theta, const double phi, const array<double, 3> euler_angles) const {
    array<double, 2> thetap_phip = euler_angle_rotation.rotate_back(array<double, 2>{theta, phi}, euler_angles);

    return (*this)(thetap_phip[0], thetap_phip[1]);
}