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

#include "SphereRejectionSampler.hh"
#include "TestUtilities.hh"

/**
 * Test by sampling from a distribution that is 1 for \f$\varphi < \pi\f$ and 0 otherwise.
 * For an optimum choice \f$W_\mathrm{max} = 1\f$, this should result in an efficiency of 
 * \f$\epsilon = 0.5\f$.
 * For the choice \f$W_\mathrm{max} = 2\f$, this should result in an efficiency of 
 * \f$\epsilon = 0.25\f$.
 */
int main(){

    SphereRejectionSampler sph_rej_sam([](const double theta, const double phi){ return phi < M_PI ? 1. : 0.; }, 1., 0);

    double efficiency = sph_rej_sam.estimate_efficiency(1e5);

    test_numerical_equality<double>(efficiency, 0.5, 1e-3);

    SphereRejectionSampler sph_rej_sam_2([](const double theta, const double phi){ return phi < M_PI ? 1. : 0.; }, 2., 0);

    efficiency = sph_rej_sam_2.estimate_efficiency(1e5);

    test_numerical_equality<double>(efficiency, 0.25, 1e-3);
}