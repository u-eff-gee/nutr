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

#pragma once

#include <cassert>
#include <cmath>
#include <stdexcept>
#include <sstream>

using std::fabs;
using std::runtime_error;
using std::stringstream;

template<typename T>
void test_numerical_equality(const T a, const T b, const T epsilon) {
	if(fabs(a-b) > fabs(epsilon)){
        stringstream error_message;
        error_message << "Test of numerical equality failed. | " << a << " - " << b << " | = " << std::scientific << fabs(a-b) << " > " << epsilon;
        throw runtime_error(error_message.str());
    };
};

template<typename T>
void test_numerical_equality(const size_t n, T* a, T* b, const T epsilon){
    for(size_t i = 0; i < n; ++i){
        test_numerical_equality<T>(a[i], b[i], epsilon);
    }
}

/**
 * \brief Check whether three numbers fulfil the triangle inequality.
 *
 * Given three numbers \f$j_1\f$, \f$j_2\f$, and \f$J\f$, check whether 
 *
 * \f[
 *	\left| j_1 - j_2 \right| <= J <= j_1 + j_2.
 * \f]
 *
 * In the present implementation, the function does not check whether  the given
 * numbers are positive, as commonly assumed when applying the triangle inequality.
 *
 * \param j1 \f$j_1\f$
 * \param j2 \f$j_2\f$
 * \param J \f$J\f$
 *
 * \return \f$ \left( \left| j_1 - j_2 \right| <= J \right) \&\& \left(J <= j_1 + j_2 \right)\f$
 */
template<typename T>
bool fulfils_triangle_inequality(const T j1, const T j2, const T J) {
	if(
		(J >= abs(j1 - j2))
		&& (J <= j1 + j2)
	)
		return true;

	return false;	
}