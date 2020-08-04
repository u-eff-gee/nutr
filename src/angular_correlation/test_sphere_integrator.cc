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

int main(){

    SphereIntegrator sph_int;

    double integral_num = sph_int([]([[maybe_unused]] double theta, [[maybe_unused]] double phi){ return 1.; }, 100000, [](double theta, [[maybe_unused]] double phi){ return theta > M_PI_2 ; });

    test_numerical_equality<double>(integral_num, 2.*M_PI, 1e-3);

}