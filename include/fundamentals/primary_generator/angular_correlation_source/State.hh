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

/**
 * \brief Enum for the possible values of the parity quantum number.
 */
enum Parity { negative, positive, parity_unknown };

/**
 * \brief Struct to store properties of a nuclear state.
 *
 * The state is characterized by its angular momentum and parity quantum number.
 */
struct State{
	/**
	 * \brief Constructor which does not take parity information
	 *
	 * Parity quantum numbers are initialized as unknown.
	 */
	State(const int t_J):
		two_J(t_J),
		parity(parity_unknown){};
	/**
	 * \brief Constructor
	 */
	State(const int t_J, const Parity p):
		two_J(t_J),
		parity(p){};
	~State() = default;

	int two_J; /**< Angular momentum quantum number in units of the reduced Planck constant times two. */
	Parity parity; /**< Parity quantum number */
};
