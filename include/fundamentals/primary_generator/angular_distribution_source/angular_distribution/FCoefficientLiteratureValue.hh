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
 * \brief Struct to store F-coefficient parameters and their values from literature data.
 */
struct FCoefficientLiteratureValue{
	/**
	 * \brief Constructor for FCoefficientLiteratureValue
	 *
	 * The order of the parameters corresponds to the order in the tables of 
	 * Ref. \cite FerentzRosenzweig1955.
	 */
	FCoefficientLiteratureValue(int t_j, int t_jp, int t_L, int t_Lp, int t_nu, double val):
		two_j(t_j),
		two_jp(t_jp),
		two_L(t_L),
		two_Lp(t_Lp),
		two_nu(t_nu),
		value(val)
	{};
	~FCoefficientLiteratureValue() = default;

	int two_j, two_jp, two_L, two_Lp, two_nu;
	double value;
};
