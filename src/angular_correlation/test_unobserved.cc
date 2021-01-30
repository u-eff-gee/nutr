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

#include <gsl/gsl_sf.h>

#include "State.hh"
#include "Transition.hh"
#include "TestUtilities.hh"
#include "W_dir_dir.hh"

/**
 * Eq. (68) in Ref. \cite AjzenbergSelove1960.
 */
double w_dir_dir_6_4_3_1(const double theta){
    return 1. 
        + 0.10204*gsl_sf_legendre_Pl(2, cos(theta))
        + 0.00907*gsl_sf_legendre_Pl(4, cos(theta));
}

/**
 * \brief Test angular correlations with unobserved intermediate transitions.
 * 
 * The present test uses an example given in Sec. 1.a.1.iii [Eqs. (66) - (68)] of 
 * Ref. \cite AjzenbergSelove1960.
 */
int main(){

	const double epsilon = 1e-4;

	double w_num{0.}, w_ana{0.};

    W_dir_dir w_dir_dir(
        State(12, parity_unknown),
        {
            {Transition(em_unknown, 4, em_unknown, 6, 0.), State(8, parity_unknown)},
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(6, parity_unknown)},            
            {Transition(em_unknown, 4, em_unknown, 6, 0.), State(2, parity_unknown)},            
        }
    );

	for(double theta = 0.; theta < M_PI; theta += 0.5){
		for(double phi = 0.; phi < M_2_PI; phi += 0.5){
			
			w_num = w_dir_dir(theta);
			w_ana = w_dir_dir_6_4_3_1(theta);

			test_numerical_equality<double>(w_num, w_ana, epsilon);
		}
	}
}