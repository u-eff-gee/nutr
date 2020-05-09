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

using std::invalid_argument;
using std::runtime_error;
using std::string;
using std::to_string;

/**
 * \brief Enum for the possible values of the parity quantum number.
 */
enum Parity : short { negative = -1, positive = 1, parity_unknown = 0 };

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
	 * 
	 * \param t_J Two times the angular momentum quantum number in units of the reduced Planck constant.
	 * 
 	 * \throw std::invalid_argument if two_J is invalid
	 */
	State(const int t_J):
		two_J(check_two_J(t_J)),
		parity(parity_unknown){};
	/**
	 * \brief Constructor
	 * 
	 * \param t_J Two times the angular momentum quantum number in units of the reduced Planck constant.
	 * \param parity Parity quantum number.
	 * 
	 * \throw std::invalid_argument if two_J is invalid
	 */
	State(const int t_J, const Parity p):
		two_J(t_J),
		parity(p){};
	~State() = default;

	int two_J; /**< Two times the angular momentum quantum number in units of the reduced Planck constant. */
	Parity parity; /**< Parity quantum number. */

	/**
	 * \brief String representation of parities.
	 * 
	 * \param parity Parity
	 * 
	 * \return "+" or "-"
	 * 
	 * \throw runtime_error if parity is neither negative (-1) or positive (1).
	 */
	string parity_str_rep(const Parity parity) const {
		
		if(parity == positive){
			return "+";
		} 
		if(parity == negative){
			return "-";
		}

		throw runtime_error("No string representation for unknown parity.");

	}

	/**
	 * \brief String representation of angular momentum quantum numbers.
	 * 
	 * \param two_J Two times the angular momentum quantum number in units of the reduced Planck constant.
	 * 
	 * \return String representation
	 */
	string spin_str_rep(const int two_J) const {

		if(two_J % 2 == 0){
			return to_string(two_J/2);
		}

		return to_string(two_J) + "/2";
		
	}

	/**
	 * \brief String representation of a State.
	 * 
	 * If the parity is unknown, it is omitted.
	 * 
	 * \param state State
	 * 
	 * \return String representation
	 */
	string str_rep() const {

		if(parity != parity_unknown){
			return spin_str_rep(two_J) + "^" + parity_str_rep(parity); 
		}

		return spin_str_rep(two_J) + parity_str_rep(parity);
	}
	
	/**
	 * \brief Ensure that given angular momentum quantum number is valid.
	 * 
	 * The reason why two_J was defined as an 'int' and not an 'unsigned int' is because the 
	 * GSL \cite Galassi2009 functions accept 'int'.
	 * 
	 * \param int two_J
	 * 
	 * \returns two_J, if it is valid
	 * 
	 * \throw std::invalid_argument if two_J is invalid
	 */
	int check_two_J(const int two_J) const {
		
		if(two_J < 0){
			throw invalid_argument("two_J must be a nonnegative integer.");
		}

		return two_J;
	}
};
