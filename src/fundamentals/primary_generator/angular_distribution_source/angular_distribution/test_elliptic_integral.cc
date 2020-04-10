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
#include <iostream>
#include <vector>

#include <gsl/gsl_math.h>
#include <gsl/gsl_sf_elljac.h>

#include "SpherePointSampler.hh"
#include "TestUtilities.hh"

using std::vector;

/**
 * \brief Literature values for complete elliptic integrals
 * 
 * The values were taken from:
 * 
 * J. M. Hammersley, Table of Complete Elliptic Integrals, J. Res. Nat. Bur. Stand. 50 (1953) 43.
 * 
 * In that reference, the parameter \f$k^{-1}\f$ is tabulated, which is related to \f$m\f$ by:
 * 
 * \f[
 *      m = \left( \frac{1}{k^{-1}} \right)^2.
 * \f]
 * 
 * In particular, the literature values include the value of the integral at \f$k = m = 1\f$, which
 * can not be handled by GSL.
 */
const vector<vector<double>> elliptic_integral_2nd_kind_literature_values{    
    { 1.00, 1.          },
    { 1.20, 1.244969258 },
    { 1.40, 1.345488787 },
    { 1.60, 1.403811262 },
    { 1.80, 1.441486811 },
    { 2.00, 1.467462209 },
};

double one_over_k_to_m(const double one_over_k){
    return pow(1./one_over_k, 2);
}

int main(){

	const double epsilon{1e-8};

    SpherePointSampler sph_pt_samp;

    double ell_int_ana = 0.;
    double ell_int_num = 0.;

    /**
     *  For \f$ 0 \leq m \leq 1 \f$, test a few literature values, in particular the one for 
     *  \f$m = 1\f$
     */
    for(auto val: elliptic_integral_2nd_kind_literature_values){
        ell_int_num = sph_pt_samp.elliptic_integral_2nd_kind_arbitrary_m(M_PI_2, one_over_k_to_m(val[0]));
        test_numerical_equality<double>(ell_int_num, val[1], epsilon);
    }

    /**
     * Test a special case with a negative value of \f$m = -\frac{1}{2}f$, using relation
     * 17.4.18 in \cite AbramowitzStegun1974.
     */

    const vector<double> u_values{
        0., 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, M_PI_2
    };

    double sn, cn, dn;

    for(auto u: u_values){
        gsl_sf_elljac_e(u*sqrt(1.5), 0.5/1.5, &sn, &cn, &dn);

        ell_int_ana = sqrt(1.5)*(
            sph_pt_samp.elliptic_integral_2nd_kind_arbitrary_m(u*sqrt(1.5), 0.5/1.5)
            - 0.5/sqrt(1.5)*sn*cn/dn
        );
        ell_int_num = sph_pt_samp.elliptic_integral_2nd_kind_arbitrary_m(u, -0.5);

        test_numerical_equality<double>(ell_int_num, ell_int_ana, epsilon);
    }

    /**
     * Test a special case with a value of \f$m = 2\f$, i.e. an absolute value larger than 1, 
     * using relation 17.4.16 in \cite AbramowitzStegun1974.
     */
    for(auto u: u_values){
        ell_int_ana = sqrt(2.)
        *sph_pt_samp.elliptic_integral_2nd_kind_arbitrary_m(u*sqrt(2.), 1./2.)
        -(2.-1.)*u;
        ell_int_num = sph_pt_samp.elliptic_integral_2nd_kind_arbitrary_m(u, 2.);

        test_numerical_equality<double>(ell_int_num, ell_int_ana, epsilon);
    }
}