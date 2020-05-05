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

#include <stdexcept>
#include <string>

#include "State.hh"

using std::invalid_argument;
using std::runtime_error;
using std::string;

/**
 * \brief Enum for the possible values of the electromagnetic character.
 */
enum EMCharacter : char { electric = -1, magnetic = 1, em_unknown = 0};

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
		em_char(em_unknown),
		two_L(check_two_L(t_L)),
		em_charp(em_unknown),
		two_Lp(check_two_L(t_Lp)),
		delta(del){
			if(two_L == two_Lp){
				throw invalid_argument("The two multipolarities for a transition may not be equal. This holds even if the coupling allows only a single multipolarity.");
			}
		};
	/**
	 * \brief Constructor
	 */
	Transition(const EMCharacter em, const int t_L, const EMCharacter emp, const int t_Lp, const double del):
		em_char(em),
		two_L(check_two_L(t_L)),
		em_charp(emp),
		two_Lp(check_two_L(t_Lp)),
		delta(del){
			if(two_L == two_Lp){
				throw invalid_argument("The two multipolarities for a transition may not be equal. This holds even if the coupling allows only a single multipolarity.");
			}
		};
	~Transition() = default;

	/**
	 * \brief String representation of electromagnetic characters.
	 * 
	 * \param em Electromagnetic character
	 * 
	 * \return "E" or "M"
	 * 
	 * \throw runtime_error if em is neither electric nor magnetic.
	 */
	static string em_str_rep(const EMCharacter em) {
		
		if(em == electric){
			return "E";
		} 
		if(em == magnetic){
			return "M";
		}

		throw runtime_error("No string representation for unknown electromagnetic character.");

	}

	/**
	 * \brief String representation of a transition between two states.
	 * 
	 * If parities or electromagnetic characters are unknown, they are omitted.
	 * At the moment, the secondary multipolarity will be shown even if the transition is pure.
	 * 
	 * \param initial_state Initial state of the transition
	 * \param final_state Final state of the transition
	 * 
	 * \return String representation
	 */
	string str_rep(const State initial_state, const State final_state) const {
	
		string string_representation = initial_state.str_rep() + " -- ( ";

		if(em_char != em_unknown){
			string_representation += em_str_rep(em_char) + to_string(two_L/2);
		} else{
			string_representation += to_string(two_L/2);
		}

		string_representation += " , ";

		if(em_charp != em_unknown){
			string_representation += em_str_rep(em_charp) + to_string(two_Lp/2);
		} else{
			string_representation += to_string(two_Lp/2);
		}

		string_representation += " ) --> " + final_state.str_rep();

		return string_representation;
	}

	EMCharacter em_char; /**< Primary electromagnetic character */
	int two_L; /**< Primary multipolarity times two */
	EMCharacter em_charp; /**< Secondary electromagnetic character */
	int two_Lp; /**< Secondary multipolarity times two */
	double delta; /**< Multipole mixing ratio */

	/**
	 * \brief Ensure that given multipolarity is valid.
	 * 
	 * The reason why two_L was defined as an 'int' and not an 'unsigned int' is because the 
	 * GSL \cite Galassi2009 functions accept 'int'.
	 * 
	 * \param int two_L
	 * 
	 * \returns two_L, if it is valid
	 * 
	 * \throw std::invalid_argument if two_L is invalid
	 */
	int check_two_L(const int two_L) const {
		
		if(two_L < 0){
			throw invalid_argument("two_L (two_Lp) must be a nonnegative integer.");
		}

		return two_L;
	}
};
