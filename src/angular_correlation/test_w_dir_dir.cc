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

#include <cmath>
#include <gsl/gsl_math.h>

#include "State.hh"
#include "TestUtilities.hh"
#include "Transition.hh"
#include "W_dir_dir.hh"

/**
 * \brief Analytical expression for the angular distribution of a \f$0 \to 1 \to 0\f$ sequence
 *
 * The expression is { Eq. (10) in \cite Kneissl1996}:
 * 
 * \f[
 * 		W\left( \theta \right) = \frac{3}{4} \left[ 1 + \cos^2 \left( \theta \right) \right].
 * \f]
 */ 
double w_dir_dir_0_1_0(const double theta){
	return 0.75*(1.+ pow(cos(theta), 2));
}

/**
 * \brief Analytical expression for the angular distribution of a \f$0 \to 2 \to 0\f$ sequence
 *
 * The expression is { Eq. (11) in \cite Kneissl1996}:
 * 
 * \f[
 * 		W\left( \theta \right) = \frac{5}{4} \left[ 1 - 3\cos^2 \left( \theta \right) + 4\cos^4 \left( \theta \right) \right].
 * \f]
 */ 
double w_dir_dir_0_2_0(const double theta){
	return (1.25 - 3.75*pow(cos(theta), 2) + 5.*pow(cos(theta), 4));
}

int main(){

	const double epsilon{1e-7};

	double w_dir_dir_num{0.};
	double w_dir_dir_ana{0.};

	W_dir_dir w_dir_dir_2(
		State(0), 
		{
			{Transition(4, 6, 0.), State(4)},
			{Transition(4, 6, 0.), State(0)},
		} 
	);

	for(double theta = 0.; theta < M_PI; theta += 0.5){
		w_dir_dir_num = w_dir_dir_2(theta);
		w_dir_dir_ana = w_dir_dir_0_2_0(theta);

		test_numerical_equality<double>(w_dir_dir_num, w_dir_dir_ana, epsilon);
	}
}
