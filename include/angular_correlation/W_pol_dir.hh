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

    Copyright (C) 2020, 2021 Udo Friman-Gayer
*/

#pragma once

#include "AlphavCoefficient.hh"
#include "AvCoefficient.hh"
#include "W_dir_dir.hh"
#include "W_gamma_gamma.hh"

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
class W_pol_dir : public W_gamma_gamma{
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
	 * The \f$\cos \left( 2\varphi \right)\f$ term introduces a dependence on the azimuthal angle \f$\varphi\f$ with respect to the polarization axis of the first photon.
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
	double operator()(const double theta, const double phi) const override;

	/**
	 * \brief Return upper limit for the pol-dir correlation.
	 * 
	 * For the pol-dir correlation, the maximum possible value cannot be calculated analytically,
	 * but an upper limit can be given.
	 * It is derived here in analogy to the limit for the dir-dir correlation in W_dir_dir.
	 * Starting from the exact expression
	 * 
	 * \f[
	 * 		\mathrm{max}_{\theta \in \left[ 0, \pi \right], \varphi \in \left[ 0, 2\pi \right]} 
	 * | W \left( \theta, \varphi \right) |
	 * \f]
	 * \f[
	 * 		= \mathrm{max}_{\theta \in \left[ 0, \pi \right], \varphi \in \left[ 0, 2\pi \right]} 
	 * | \sum_\nu A_v \left( L_1, L_1^\prime, j_1, j_2 \right) A_v \left( L_2, L_2^\prime, j_3, j_2 \right)  P_\nu \left[ \cos \left( \theta \right) \right] + \left( \pm \right)_{L_1^\prime} \cos \left( 2 \varphi \right) \sum_\nu \kappa_\nu \left( L_1, L_1^\prime \right) A_v \left( L_1, L_1^\prime, j_1, j_2 \right) A_v \left( L_2, L_2^\prime, j_3, j_2 \right) P_\nu^{\left( 2 \right)} \left[ \cos \left( \theta \right) \right] |,
	 * \f]
	 * 
	 * using the triangle inequality and properties of the absolute value, one obtains:
	 * 
	 * \f[
	 * 		\mathrm{max}_{\theta \in \left[ 0, \pi \right], \varphi \in \left[ 0, 2\pi \right]} 
	 * | W \left( \theta, \varphi \right) | 
	 * \f]
	 * \f[
	 * 		\leq
	 * \sum_\nu | A_v \left( L_1, L_1^\prime, j_1, j_2 \right) A_v \left( L_2, L_2^\prime, j_3, j_2 \right) | \mathrm{max}_{\theta \in \left[ 0, \pi \right]} P_\nu \left[ \cos \left( \theta \right) \right] + \lbrace \mathrm{max}_{\varphi \in \left[ 0, 2\pi \right]} \cos \left( 2 \varphi \right) \rbrace \sum_\nu | \kappa_\nu \left( L_1, L_1^\prime \right) A_v \left( L_1, L_1^\prime, j_1, j_2 \right) A_v \left( L_2, L_2^\prime, j_3, j_2 \right)| \mathrm{max}_{\theta \in \left[ 0, \pi \right]} | P_\nu^{\left( 2 \right)} \left[ \cos \left( \theta \right) \right] |.
	 * \f]
	 * 
	 * The maximum absolute value of a cosine function of a real scalar is 1.
	 * For the associated Legendre Polynomial \f$P_\nu^{\left( 2 \right)}\f$, an upper limit is 
	 * given by Lohöfer {Eq. (22) in \cite Lohoefer1998}:
	 * 
	 * \f[
	 * 		\max_{\cos \left( \theta \right) \in \left[ -1, 1 \right]} | P_\nu^{\left( 2 \right)} \left[ \cos \left( \theta \right) \right] | < \frac{4}{\pi^{3/4}} \sqrt{\frac{\left( \nu + 2 \right)!}{\left( \nu-2 \right)!}}.
	 * \f]
	 * 
	 * Using these results and the upper limit for the Legendre polynomial \f$P_\nu\f$ from W_dir_dir 
	 * (which is simply 1), the upper limit returned by this function is given by:
	 * 
	 * \f[
	 * 		\sum_\nu | A_v \left( L_1, L_1^\prime, j_1, j_2 \right) A_v \left( L_2, L_2^\prime, j_3, j_2 \right) | + \sum_\nu | \kappa_\nu \left( L_1, L_1^\prime \right) A_v \left( L_1, L_1^\prime, j_1, j_2 \right) A_v \left( L_2, L_2^\prime, j_3, j_2 \right)| \frac{4}{\pi^{3/4}} \sqrt{\frac{\left( \nu + 2 \right)!}{\left( \nu-2 \right)!}}
	 * \f]
	 * 
	 * Note that the first part of the sum is the upper limit for the dir-dir correlation.
	 * 
	 * \return \f$\mathrm{max}_{\theta \in \left[ 0, \pi \right], \varphi \in \left[ 0, 2\pi \right]} 
	 * | W \left( \theta, \varphi \right) | \f$, or an upper limit for this quantity.
	 * If no useful upper limit can be given or if there is no limit, a negative number is returned.
	 */
	double get_upper_limit() const override;

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

};
