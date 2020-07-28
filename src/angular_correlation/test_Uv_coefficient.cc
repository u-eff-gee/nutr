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

#include "UvCoefficient.hh"
#include "TestUtilities.hh"

int main(){

    // Test the calculation of \f$U_\nu\f$ coefficients using the example given in Sec. 1.a.1.iii
    // [Eqs. (66) - (68)] of Ref. \cite AjzenbergSelove1960, which gives the values of two
    // coefficients explicitly.

	UvCoefficient uv_coef;
	const double epsilon = 1e-4;

    // Test the coefficient \f$U_2\f$
	test_numerical_equality<double>(
        uv_coef(4, 8, 2, 6), 
        0.90469, epsilon);

    // Test the coefficient \f$U_4\f$
	test_numerical_equality<double>(
        uv_coef(8, 8, 2, 6), 
        0.68138, epsilon);

}