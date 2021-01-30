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

    Copyright (C) 2020, 2021 Udo Friman-Gayer
*/

#include <array>
#include <vector>

#include <gsl/gsl_math.h>

#include "SphereIntegrator.hh"

using std::array;
using std::vector;

double SphereIntegrator::operator()(double f(double theta, double phi), const unsigned int n, bool is_in_omega(double theta, double phi)) {

    array<vector<double>, 2> theta_phi = sph_poi_samp.sample(n);

    double integral = 0.;

    for(size_t i = 0; i < (size_t) n; ++i){
        if(is_in_omega(theta_phi[0][i], theta_phi[1][i])){
            integral += f(theta_phi[0][i], theta_phi[1][i]);
        }
    }

    return 4.*M_PI/(double) n*integral;
}