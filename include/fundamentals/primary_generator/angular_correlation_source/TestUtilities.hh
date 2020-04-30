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

