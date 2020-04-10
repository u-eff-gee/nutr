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

#include <gsl/gsl_sf_ellint.h>
#include <gsl/gsl_sf_elljac.h>

#include "SpherePointSampler.hh"

double SpherePointSampler::elliptic_integral_2nd_kind_arbitrary_m(const double u, const double m) const {
    if(m < 0.){
        const double abs_m = fabs(m);
        const double sqrt_1_plus_m = sqrt(1. + abs_m);
        const double m_over_1_plus_m = abs_m/(1. + abs_m);

        double sn, cn, dn;
        gsl_sf_elljac_e(u*sqrt_1_plus_m, m_over_1_plus_m, &sn, &cn, &dn);

        return sqrt_1_plus_m*(
            elliptic_integral_2nd_kind_arbitrary_m(u*sqrt_1_plus_m, m_over_1_plus_m)
            -abs_m/sqrt_1_plus_m*sn*cn/dn
        );
    }

    if(m > 1.){
        const double sqrt_m = sqrt(m);

        return sqrt_m*elliptic_integral_2nd_kind_arbitrary_m(u*sqrt_m, 1./m)-(m-1.)*u;
    }

    // The GSL function cannot handle this case.
    if(m == 1.)
        return 1.;

    return gsl_sf_ellint_E(u, sqrt(m), GSL_PREC_DOUBLE);
}