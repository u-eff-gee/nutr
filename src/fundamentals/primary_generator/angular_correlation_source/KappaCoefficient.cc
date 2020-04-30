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
#include <stdexcept>

#include <gsl/gsl_sf.h>

#include "KappaCoefficient.hh"

using std::invalid_argument;

double KappaCoefficient::operator()(const int two_nu, const int two_L, const double two_Lp) const {
    
    const int nu = two_nu/2;
    if(nu < 2){
        throw invalid_argument("nu must be an integer larger than 1.");
    }

    /*
        The expression for \f$\kappa_\nu\f$ given in the literature 
        {see, e.g. Eq. (I-7) in \cite FaggHanna1959} use Clebsch-Gordan (CG) coefficients, but GSL
        only provides Wigner-3j symbols.
        The CG coefficients include an additional factor of {see, e.g., Eq. (C.12) in \cite Messiah19922}

        \f[
            \left( -1 \right)^{-L + L^\prime - M} \sqrt{2 \nu + 1},
        \f]

        where \f$M\f$ denotes the total magnetic quantum number.
        In the present case, all factors cancel out, and the Wigner-3j symbols can be used.
        Note, however, that \f$M\f$ changes its sign when going from the CG coefficient 
        to the Wigner-3j symbol.
    */
    return -sqrt((double) gsl_sf_fact(nu-2)/(double) gsl_sf_fact(nu+2))
    *gsl_sf_coupling_3j(two_L, two_Lp, two_nu, 2, 2, -4)
    /gsl_sf_coupling_3j(two_L, two_Lp, two_nu, 2, -2, 0);
}