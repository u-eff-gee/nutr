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
 * \brief Enum for the possible values of the electromagnetic character.
 */
enum EMCharacter : int { electric = -1, magnetic = 1, em_unknown = 0};

/**
 * \brief Struct to store properties of a transition between nuclear states.
 *
 * The transition can have two different multipolarities with their associated electromagnetic
 * character, whose relative intensity is given by the multipole mixing ratio.
 */
struct Transition{
	/**
	 * \brief Constructor which does not take information about the electromagnetic character
	 *
	 * The electromagnetic characters are initialized as unknown.
	 */
	Transition(const int t_L, const int t_Lp, const double del):
		two_L(t_L),
		two_Lp(t_Lp),
		delta(del),
		em_char(em_unknown),
		em_charp(em_unknown){};
	/**
	 * \brief Constructor
	 */
	Transition(const EMCharacter em, const int t_L, const EMCharacter emp, const int t_Lp, const double del):
		em_char(em),
		two_L(t_L),
		em_charp(emp),
		two_Lp(t_Lp),
		delta(del){};
	~Transition() = default;

	double delta; /**< Multipole mixing ratio */
	int two_L; /**< Primary multipolarity times two */
	int two_Lp; /**< Secondary multipolarity times two */
	EMCharacter em_char; /**< Primary electromagnetic character */
	EMCharacter em_charp; /**< Secondary electromagnetic character */
};
