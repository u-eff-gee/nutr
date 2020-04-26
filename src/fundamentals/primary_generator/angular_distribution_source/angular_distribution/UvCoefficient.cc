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

#include <cmath>
#include <gsl/gsl_sf.h>

#include "UvCoefficient.hh"

double UvCoefficient::operator()(const unsigned int two_nu, const int two_j, const int two_L, const int two_jp) const {

    const int phase_factor = (two_jp - two_j - two_L)/2 % 2 == 0 ? 1 : -1;

    return phase_factor
    *sqrt(
        (two_jp + 1)*(two_j + 1)
    )
    *gsl_sf_coupling_6j(
        two_j, two_j, two_nu,
        two_jp, two_jp, two_L);
}