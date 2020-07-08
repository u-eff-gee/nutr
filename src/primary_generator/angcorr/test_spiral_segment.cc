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

#include <vector>

#include <gsl/gsl_math.h>

#include "SpherePointSampler.hh"
#include "TestUtilities.hh"

using std::vector;

/**
 * Test the calculation of the length of a spiral segment by comparison to a simple (and inefficient)
 * algorithm that determines the same quantity from a piecewise linear interpolation 
 * of the spiral.
 */
int main(){

	const double epsilon{1e-4};

    SpherePointSampler sph_pt_samp;

    vector<double> c_values{2., 20.};

    const size_t n_Theta = 10;
    const double Theta_increment = M_PI/((double) n_Theta - 1);
    vector<double> Theta_values(n_Theta, 0.);

    const unsigned int n_interpolation_points = 1e4;

    for(size_t i = 0; i < n_Theta; ++i){
        Theta_values[i] = i*Theta_increment;
    }

    double spiral_segment_elliptic_integral{0.}, spiral_segment_linear_interpolation{0.};
    for(auto c: c_values){
        for(auto Theta: Theta_values){
            spiral_segment_elliptic_integral = sph_pt_samp.segment_length(Theta, c);
            spiral_segment_linear_interpolation = sph_pt_samp.segment_length_linear_interpolation(Theta, c, n_interpolation_points);

            test_numerical_equality<double>(spiral_segment_elliptic_integral, spiral_segment_linear_interpolation, epsilon);
        }
    }
}