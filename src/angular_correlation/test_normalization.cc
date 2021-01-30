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

    const unsigned int n = 10000;

    // Test direction-direction correlation with pure transition
    // double integral_num = sph_int([](double theta, double phi){ 
    double integral_num = sph_int([](double theta, [[maybe_unused]] double phi){ 
        
        W_dir_dir w_dir_dir(
            State(0, parity_unknown),
            {
                {Transition(em_unknown, 2, em_unknown, 4, 0.), State(2, parity_unknown)},
                {Transition(em_unknown, 2, em_unknown, 4, 0.), State(4, parity_unknown)}
            }
        );        
        return w_dir_dir(theta);

    }, n, []([[maybe_unused]] double theta, [[maybe_unused]] double phi){ return true ; });

    test_numerical_equality<double>(integral_num, normalization, 1e-3);

    // Test direction-direction correlation with mixed transition
    integral_num = sph_int([](double theta, [[maybe_unused]] double phi){ 
        
        W_dir_dir w_dir_dir(
            State(0, parity_unknown),
            {
                {Transition(em_unknown, 2, em_unknown, 4, 0.), State(2, parity_unknown)},
                {Transition(em_unknown, 2, em_unknown, 4, 2.),
                State(4, parity_unknown)}
            }
        );        
        return w_dir_dir(theta);

    }, n, []([[maybe_unused]] double theta, [[maybe_unused]] double phi){ return true ; });

    test_numerical_equality<double>(integral_num, normalization, 1e-3);

    // Test direction-direction correlation with mixed unobserved transition
    integral_num = sph_int([](double theta, [[maybe_unused]] double phi){ 
        
        W_dir_dir w_dir_dir(
            State(0, parity_unknown),
            {
                {Transition(em_unknown, 2, em_unknown, 4, 0.), State(2, parity_unknown)},
                {Transition(em_unknown, 2, em_unknown, 4, 2.), State(2, parity_unknown)},
                {Transition(em_unknown, 2, em_unknown, 4, 0.),
                State(4, parity_unknown)}
            }
        );        
        return w_dir_dir(theta);

    }, n, []([[maybe_unused]] double theta, [[maybe_unused]] double phi){ return true ; });

    test_numerical_equality<double>(integral_num, normalization, 1e-3);

    // Test polarization-direction correlation with mixed transition
    integral_num = sph_int([](double theta, double phi){ 

        W_pol_dir w_pol_dir(
            State(3, positive),
            {
                {Transition(magnetic, 6, electric, 8, 2.), State(9, positive)},
                {Transition(magnetic, 2, electric, 4, -2.), State(7, positive)}
            }
        );        
        return w_pol_dir(theta, phi);

    }, n, []([[maybe_unused]] double theta, [[maybe_unused]] double phi){ return true ; });

    test_numerical_equality<double>(integral_num, normalization, 1e-3);

    // Test polarization-direction correlation with mixed unobserved transition
    integral_num = sph_int([](double theta, double phi){ 

        W_pol_dir w_pol_dir(
            State(3, positive),
            {
                {Transition(magnetic, 6, electric, 8, 0.), State(9, positive)},
                {Transition(magnetic, 2, electric, 4, 2.), State(7, positive)},
                {Transition(magnetic, 2, electric, 4, 0.), State(7, positive)}
            }
        );

        return w_pol_dir(theta, phi);

    }, n, []([[maybe_unused]] double theta, [[maybe_unused]] double phi){ return true ; });

    test_numerical_equality<double>(integral_num, normalization, 1e-3);
}