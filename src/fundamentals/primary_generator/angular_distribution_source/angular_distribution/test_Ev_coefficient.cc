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

#include "EvCoefficient.hh"
#include "TestUtilities.hh"
#include "Transition.hh"

int main(){

	EvCoefficient ev_coef;
	const double epsilon = 1e-4;

	double ev_coef_num{0.};

	// Eq. (88b) in \cite Biedenharn1960
	ev_coef_num = ev_coef(4, electric, 4, magnetic, 6, 7, 3, 0.);
	test_numerical_equality<double>(ev_coef_num, 0.07143, epsilon);
	
	// Eq. (89b) in \cite Biedenharn1960
	ev_coef_num = ev_coef(4, magnetic, 2, electric, 4, 3, 3, 1.);
	test_numerical_equality<double>(ev_coef_num, 0.20000 - 0.25820, epsilon);
}
