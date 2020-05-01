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

#include "AlphavCoefficient.hh"
#include "AvCoefficient.hh"
#include "State.hh"
#include "Transition.hh"
#include "W_dir_dir.hh"

/**
 * \brief Class for a polarization-direction (pol-dir) correlation
 *
 * 
 * Compared to the dir-dir correlation (see the documentation of the W_dir_dir class for more 
 * information), it is assumed that both the direction and the polarization of the first photon 
 * are known, and that the direction of the second photon is measured.
 * 
 * The class uses Eqs. (I-1), (I-1'), (I-2), and (I-4) of Ref. \cite FaggHanna1959 with a similar 
 * notation (Note that Ref. \cite FaggHanna1959 assumes that the polarization information is given 
 * for the exit channel.
 * The case of an incoming polarized photon, which is completely analogous, is shown in 
 * Ref. \cite Kneissl1996 and also used here.).
 * It makes use of the dir-dir correlation which is implemented in the class W_dir_dir.
 * 
 * The pol-dir correlation is normalized to \f$4 \pi\f$ like the dir-dir correlation.
 */
class W_pol_dir{
public:
	/**
	 * \brief Constructor
	 * 
	 * \param ini_sta Oriented intial state.
	 * \param cas_ste Steps of the cascade which follow the exciation of the initial state.
	 * Each step is a pair of a transition and the state which is populated by that transition.
	 */
	W_pol_dir(const State &ini_sta, const vector<pair<Transition, State>> cas_ste);
	/**
	 * \brief Destructor
	 */
	~W_pol_dir() = default;

	/**
	 * \brief Return value of the pol-dir correlation at angles \f$\theta\f$ and \f$\varphi\f$
	 * 
	 * In Eq. (I-4) of Ref. \cite FaggHanna1959, it is shown that the pol-dir correlation 
	 * can be obtained from the dir-dir correlation [Eq. (I-1)] by the following replacement 
	 * (note that the polarization is assumed to be known for the transition \f$j_1 \to j_2\f$ here, 
	 * instead of \f$j_2 \to j_3\f$):
	 * 
	 * \f[
	 * 		W \left( \theta, \varphi \right) = \sum_\nu A_v \left( L_1, L_1^\prime, j_1, j_2 \right) A_v \left( L_2, L_2^\prime, j_3, j_2 \right) \underbrace{ P_\nu \left[ \cos \left( \theta \right) \right] }_{ P_\nu \left[ \cos \left( \theta \right) \right] + \left( \pm \right)_{L_1^\prime} \cos \left( 2 \varphi \right) \kappa_\nu \left( L_1, L_1^\prime \right) P_\nu^{\left( 2 \right)} \left[ \cos \left( \theta \right) \right]}.
	 * \f]
	 * 
	 * Compared to the dir-dir correlation, the expression contains a term whose sign is 
	 * determined by the electromagnetic charactor of the alternative multipolarity of the first 
	 * transition.
	 * Explicitly, \f$\left( \pm \right)_{L_1^\prime} = +1\f$ if it has electric character, and 
	 * \f$\left( \pm \right)_{L_1^\prime} = -1\f$ if it has magnetic character.
	 * The \f$\cos \left( 2\varphi \right)\f$ term introduces a dependence on the azimuthal angle $\varphi$ with respect to the polarization axis of the first photon.
	 * In Ref. \cite Kneissl1996, the symbol \f$\kappa_\nu \left( L_1, L_1^\prime \right)\f$ is 
	 * called the 'polarization coefficient'.
	 * It is defined in Eq. (I-7) of Ref. \cite FaggHanna1959.
	 * The symbol \f$P_\nu^{\left( 2 \right)}\f$ denotes an associated Legendre polynomial 
	 * of degree \f$\nu\f$ and order 2.
	 * 
	 * If unobserved intermediate transitions are present, the equations above can be generalized
	 * by inserting the respective \f$U_\nu\f$ coefficients.
	 * See also the classes W_dir_dir and UvCoefficient for more information.
	 * 
	 * \param theta Polar angle between the direction of the incoming and 
	 * the outgoing photon in radians.
	 * \param phi Azimuthal angle between the polarization axis of the first photon and the direction of the
	 * outgoing photon in radians.
	 * 
	 * \return \f$W \left( \theta, \varphi \right)\f$
	 */
	double operator()(const double theta, const double phi) const;

protected:
	/**
	 * \brief Calculate the set of expansion coefficients for the pol-dir correlation.
	 * 
	 * The sum over \f$\nu\f$ in Eqs. (I-1), (I-1'), and (I-8) of \cite FaggHanna1959 contains 
	 * products of \f$A_\nu\f$, \f$\alpha_\nu\f$ and potentially also \f$U_\nu\f$ coefficients.
	 * 
	 * This function calls the function W_pol_dir::calculate_expansion_coefficients_alphav_Av() to 
	 * calculate the products of \f$\alpha_\nu\f$ and \f$A_\nu\f$ coefficients from \f$\nu = 0\f$ 
	 * up to, and including, 
	 * a maximum value of \f$\nu_\mathrm{max}\f$ for a given set of quantum numbers.
	 * If the cascade contains more than two transitions, the corresponding products of 
	 * \f$U_\nu\f$ coefficients for the same values of \f$\nu\f$ are calculated by the 
	 * W_dir_dir::calculate_expansion_coefficients_Av() function.
	 * This function merges the output of the two functions.
	 * 
	 * \return \f$\alpha_\nu (1) A_\nu (2)\f$ (\f$n=3\f$) or \f$\alpha_\nu (1) U_\nu (2) ... A_\nu (n)\f$ (\f$n>3\f$) for \f$ \nu \in \lbrace 0, ..., \nu_\mathrm{max} \rbrace,~ \nu~\mathrm{even} \f$ sorted by increasing values
	 *  of \f$\nu\f$ in a std::vector.
	 */	
	vector<double> calculate_expansion_coefficients() const;

	/**
	 * \brief Calculate set of products of \f$\alpha_\nu A_\nu \f$ coefficients
	 * 
	 * The sum over \f$\nu\f$ in the polarization-related part in Eq. (I-8) of 
	 * \cite FaggHanna1959 contains products of \f$\alpha_\nu\f$ and \f$A_\nu\f$ coefficients 
	 * of the form:
	 * 
	 * \f[
	 * 		\alpha_\nu \left( L_1, L_1^\prime, j_1, j_2, \delta_1 \right) 
	 * 		A_v \left( L_{n-1}, L_{n-1}^\prime, j_n, j_{n-1}, \delta_{n-1} \right).
	 * \f]
	 * 
	 * This function calculates all products from \f$\nu = 2\f$ up to, and including, a maximum 
	 * value of \f$\nu_\mathrm{max}\f$ for a given set of quantum numbers, and returns them 
	 * as a vector.
	 * 
	 * \return \f$\alpha_\nu (1) A_\nu (n)\f$ for \f$ \nu \in \lbrace 0, ..., \nu_\mathrm{max} \rbrace,~ \nu~\mathrm{even} \f$ sorted by increasing values
	 *  of \f$\nu\f$ in a std::vector.
 	 */
	vector<double> calculate_expansion_coefficients_alphav_Av() const;

	const AvCoefficient av_coef; /**< Instance of the AvCoefficient class */
	const AlphavCoefficient alphav_coef; /**< Instance of the AlphavCoefficient class */
	vector<double> expansion_coefficients; /**< Vector to store expansion coefficients */
	const W_dir_dir w_dir_dir; /**< Instance of the W_dir_dir class */

	const State initial_state; /**< Initial state */
	/** 
	 * Steps of the gamma-ray cascade following an excitation.
	 * Each step consists of an electromagnetic transition and a state which is populated by 
	 * that transition.
	 */
	const vector<pair<Transition, State>> cascade_steps;
	const size_t n_cascade_steps; /**< Number of transitions in the cascade. */

	int two_nu_max; /**< Maximum value of \f$2 \nu\f$ for which the coefficients do not vanish */

};
