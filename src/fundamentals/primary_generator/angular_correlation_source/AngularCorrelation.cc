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

extern "C" double angular_correlation(const double theta, const double phi, const size_t n_cas_ste, int* two_J, int* par, int* em_char, int* two_L, int* em_charp, int* two_Lp, double* delta){

    State initial_state{two_J[0], (Parity) par[0]};
    vector<pair<Transition, State>> cascade_steps;

    for(size_t i = 0; i < n_cas_ste; ++i){
        cascade_steps.push_back(
            {
                Transition{(EMCharacter) em_char[i], two_L[i], (EMCharacter) em_charp[i], two_L[i], delta[i]},
                State{two_J[i+1], (Parity) par[i+1]}
            }
        );
    }

    AngularCorrelation ang_corr(initial_state, cascade_steps);

    return ang_corr(theta, phi);
}