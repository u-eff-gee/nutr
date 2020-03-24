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
