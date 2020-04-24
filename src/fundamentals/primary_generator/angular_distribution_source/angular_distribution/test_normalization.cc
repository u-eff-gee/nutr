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

#include <gsl/gsl_math.h>

#include "SphereIntegrator.hh"
#include "TestUtilities.hh"
#include "W_dir_dir.hh"
#include "W_pol_dir.hh"
/**
 * \brief Test the normalization of the angular correlation functions.
 * 
 * An integral over the entire solid angle should evaluate to \f$4 \pi\f$.
 * 
 * Test some randomly selected direction-direction and polarization-direction correlations
 * for this property using the SphereIntegrator class.
 * 
 * The method of passing the correlation functions to the SphereIntegrator is quite inefficient,
 * since it creates a new function object every time the function is called.
 * This way, no use can be made of precalculated coefficients.
 */
int main(){

    SphereIntegrator sph_int;

    const double normalization = 4.*M_PI;

    const unsigned int n = 2000;

    double integral_num = sph_int([](double theta, double phi){ 
        
        W_dir_dir w_dir_dir(
            State(0, parity_unknown),
            Transition(em_unknown, 2, em_unknown, 4, 0.),
            State(2, parity_unknown),
            Transition(em_unknown, 2, em_unknown, 4, 0.),
            State(4, parity_unknown)
        );        
        return w_dir_dir(theta);

    }, n, [](double theta, double phi){ return true ; });

    test_numerical_equality<double>(integral_num, normalization, 1e-2);

    integral_num = sph_int([](double theta, double phi){ 
        
        W_dir_dir w_dir_dir(
            State(0, parity_unknown),
            Transition(em_unknown, 2, em_unknown, 4, 0.),
            State(2, parity_unknown),
            Transition(em_unknown, 2, em_unknown, 4, 2.),
            State(4, parity_unknown)
        );        
        return w_dir_dir(theta);

    }, n, [](double theta, double phi){ return true ; });

    test_numerical_equality<double>(integral_num, normalization, 1e-2);

    integral_num = sph_int([](double theta, double phi){ 
        
        W_pol_dir w_pol_dir(
            State(3, positive),
            Transition(electric, 4, magnetic, 6, 2.),
            State(9, positive),
            Transition(magnetic, 2, electric, 4, -2.),
            State(7, positive)
        );        
        return w_pol_dir(theta, phi);

    }, n, [](double theta, double phi){ return true ; });
}