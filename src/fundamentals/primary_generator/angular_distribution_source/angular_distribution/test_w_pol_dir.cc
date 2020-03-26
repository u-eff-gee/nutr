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
#include <gsl/gsl_math.h>
#include <gsl/gsl_sf.h>
#include <iostream>

#include "State.hh"
#include "TestUtilities.hh"
#include "Transition.hh"
#include "W_pol_dir.hh"

/**
 * \brief Analytical expression for the angular distribution of a 
 * \f$0^+ \to 1^\pm \to 0^+\f$ sequence
 *
 * The expression is {Eq. (1) in \cite Pietralla2001}:
 * 
 * \f[
 * 		W\left( \theta \right) = \frac{3}{4} \left[ 1 + \cos^2 \left( \theta \right) \right].
 * \f]
 */ 
double w_pol_dir_0_1_0(const double theta, const double phi, const EMCharacter em){

	int parity_sign{1};
	if(em == electric){
		parity_sign = -1;
	}

	return 1.+0.5*(
		gsl_sf_legendre_Pl(2, cos(theta))
		+0.5*parity_sign*cos(2.*phi)*gsl_sf_legendre_Plm(2, 2, cos(theta))
	);
}

int main(){

	const double epsilon{1e-7};

	double w_pol_dir_num{0.};
	double w_pol_dir_ana{0.};

	W_pol_dir w_pol_dir_e1(
		State(0, positive), Transition(electric, 2, magnetic, 4, 0.), 
		State(2, positive), Transition(electric, 2, magnetic, 4, 0.),
		State(0, positive)
	);

	W_pol_dir w_pol_dir_m1(
		State(0, positive), Transition(magnetic, 2, electric, 4, 0.), 
		State(2, positive), Transition(magnetic, 2, electric, 4, 0.),
		State(0, positive)
	);

	for(double theta = 0.; theta < M_PI; theta += 0.5){
		for(double phi = 0.; phi < M_2_PI; phi += 0.5){
			
			w_pol_dir_num = w_pol_dir_e1(theta, phi);
			w_pol_dir_ana = w_pol_dir_0_1_0(theta, phi, electric);

			test_numerical_equality<double>(w_pol_dir_num, w_pol_dir_ana, epsilon);

			w_pol_dir_num = w_pol_dir_m1(theta, phi);
			w_pol_dir_ana = w_pol_dir_0_1_0(theta, phi, magnetic);

			test_numerical_equality<double>(w_pol_dir_num, w_pol_dir_ana, epsilon);
		}
	}
}
