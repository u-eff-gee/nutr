#pragma once

/**
 * \brief Enum for the possible values of the electromagnetic character.
 */
enum EMCharacter { electric, magnetic, em_unknown };

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
