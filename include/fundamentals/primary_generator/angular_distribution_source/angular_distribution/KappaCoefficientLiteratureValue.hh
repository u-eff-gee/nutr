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

#include <vector>

using std::vector;

/**
 * \brief Struct to store \f$\kappa_\nu\f$ coefficient parameters and their values from literature data.
 */
struct KappaCoefficientLiteratureValue{
	/**
	 * \brief Constructor for KappaCoefficientLiteratureValue
	 *
	 */
	KappaCoefficientLiteratureValue(int t_nu, int t_L, int t_Lp, double val):
		two_nu(t_nu),
		two_L(t_L),
		two_Lp(t_Lp),
		value(val)
	{};
	~KappaCoefficientLiteratureValue() = default;

	int two_nu, two_L, two_Lp;
	double value;
};
