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
#include <gsl/gsl_sf.h>

#include "FCoefficient.hh"
#include "TestUtilities.hh"

double FCoefficient::operator()(const int two_nu, const int two_L, const int two_Lp, const int two_j1, const int two_j) const {
	double wigner3j{
		gsl_sf_coupling_3j(
			two_L, two_Lp, two_nu,
			2, -2, 0)
	};

	// Shortcut to avoid further calculations.
	if(wigner3j == 0.) return 0.;

	double wigner6j{
		gsl_sf_coupling_6j(
			two_j, two_j, two_nu,
			two_Lp, two_L, two_j1)
	};

	// Another shortcut
	if(wigner6j == 0.) return 0.;

	return 
		pow(-1, (two_j1 + two_j)/2 - 1)
		*sqrt((two_L + 1)*(two_Lp + 1)*(two_j + 1)*(two_nu + 1))*wigner3j*wigner6j;
}

bool FCoefficient::is_nonzero(const int two_nu, const int two_L, const int two_Lp, const int two_j1, const int two_j){
	if(
		cg_is_nonzero(
			two_L, two_Lp, two_nu,
			2, -2, 0)
		&& racah_is_nonzero(
			two_j, two_j, two_nu,
			two_Lp, two_L, two_j1
			)
	)
		return true;

	return false;
}

bool FCoefficient::cg_is_nonzero(const int two_j1, const int two_j2, const int two_J, const int two_m1, const int two_m2, const int two_M) const {

	// Maximum projection of angular momentum.
	if(
		   (two_m1 > two_j1)
		|| (two_m2 > two_j2)
		|| (two_M  > two_J)
	)
		return false;

	// Conservation of angular momentum for magnetic quantum number.
	if(two_m1 + two_m2 != two_M)
		return false;

	// Triangle inequality for coupling.
	if(!fulfils_triangle_inequality<int>(two_j1, two_j2, two_J))
		return false;

	return true;
}

bool FCoefficient::racah_is_nonzero(const int two_j1, const int two_j2, const int two_j3, const int two_J1, const int two_J2, const int two_J3) const {

	if(
		   !sum_is_even(two_j1, two_j2, two_j3)
		|| !fulfils_triangle_inequality<int>(two_j1, two_j2, two_j3)
		|| !sum_is_even(two_j1, two_J2, two_J3)
		|| !fulfils_triangle_inequality<int>(two_j1, two_J2, two_J3)
		|| !sum_is_even(two_J1, two_j2, two_J3)
		|| !fulfils_triangle_inequality<int>(two_J1, two_j2, two_J3)
		|| !sum_is_even(two_J1, two_J2, two_j3)
		|| !fulfils_triangle_inequality<int>(two_J1, two_J2, two_j3)
	)
		return false;

	return true;
}