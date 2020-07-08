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

#include "AvCoefficient.hh"
#include "UvCoefficient.hh"
#include "W_gamma_gamma.hh"

/**
 * \brief Class for a direction-direction (dir-dir) correlation
 *
 * Evaluates the angular correlation between two photons which are emitted from an oriented nuclear
 * state.
 * Without loss of generality, the orientation is assumed to be in the z direction.
 * It may result from a nuclear reaction of the type 
 * \f$\left( \gamma, \gamma^\prime \right)\f$, in which a nucleus in an initial state with the 
 * total angular momentum quantum number ('spin') \f$j_1\f$ absorbs a photon from a beam 
 * that travels in positive z direction to render it in an 
 * excited state with the spin \f$j_2\f$, 
 * and emits another photon in a transition to a lower-lying state with the spin \f$j_n\f$.
 * An arbitrary number of intermediate states \f$j_i\f$ with labels \f$1 < i < n\f$ may be
 * populated in the decay process.
 * It is assumed that only the two photons associated with the transitions 
 * \f$j_1 \to j_2\f$ and \f$j_{n-1} \to j_n\f$ are observed.
 * Furthermore, the restriction to a dir-dir correlation implies that only information about the
 * direction of the photons is available, but not about their polarization.
 * In particular, this means that the dir-dir correlation does not depend on the parity
 * quantum number of the involved nuclear states, which translates into an independence of the
 * azimuthal angle of the emitted photon.
 * 
 * The class uses Eqs. (I-1), (I-1') and (I-2) of Ref. \cite FaggHanna1959 with a similar notation.
 * Note that Eq. (I-2) already includes the assumption that only two multipoles contribute
 * to any of the two transitions, and that the spin of the intermediate states is known.
 * The process can be denoted as {this notation is similar to the one used by Biedenharn 
 * \cite AjzenbergSelove1960}:
 * 
 * \f[
 * 		j_1 \left( \begin{array}{c} L_1 \\ L_1^\prime \end{array} \right) j_2 \left( \begin{array}{c} L_2 \\ L_2^\prime \end{array} \right) ... j_n,
 * \f]
 * 
 * where the dots may represent an arbitrary number of intermediate transitions.
 * The entire sequence of transitions, whose first and last transition are observed, is called
 * a cascade.
 * 
 * The dir-dir correlation is normalized to \f$4 \pi\f$ here compared to Ref. \cite FaggHanna1959
 * {see below Eq. (I-2) therein} by dividing through the \f$\left( 1 + \delta^2 \right)\f$ factors.
 */
class W_dir_dir : public W_gamma_gamma{
public:
	/**
	 * \brief Constructor
	 * 
	 * \param ini_sta Oriented intial state.
	 * \param cas_ste Steps of the cascade which follow the exciation of the initial state.
	 * Each step is a pair of a transition and the state which is populated by that transition.
	 */
	W_dir_dir(const State &ini_sta, const vector<pair<Transition, State>> cas_ste);
	/**
	 * \brief Destructor
	 */
	~W_dir_dir() = default;

	/**
	 * \brief Return value of the dir-dir correlation at an angle \f$\theta\f$
	 * 
	 * For a two-step cascade, it is given by the following expression 
	 * {Eqs. (I-1) and (I-2) in Ref. \cite FaggHanna1959}:
	 * 
	 * \f[
	 * 		W \left( \theta, \varphi \right) = W \left( \theta \right) = \sum_\nu A_v \left( L_1, L_1^\prime, j_1, j_2 \right) A_v \left( L_2, L_2^\prime, j_3, j_2 \right) P_\nu \left[ \cos \left( \theta \right) \right].
	 * \f]
	 * 
	 * The first transition proceeds via the multipolarities \f$L_1\f$ and \f$L_1^\prime\f$,
	 * while the second has the multipolarities \f$L_2\f$ and \f$L_2^\prime\f$.
	 * The symbol \f$P_v\f$ denotes a Legendre polynomial of the degree \f$\nu\f$.
	 * It incorporates the dependence on the polar angle \f$\theta\f$, while the dependence on the
	 * nuclear properties is encoded in the \f$A_\nu\f$ coefficients.
	 * 
	 * For a cascade with more than 2 steps, the so-called \f$U_\nu\f$ coefficients enter the 
	 * previous equation to take into account the deorientation due to the addition decays
	 * {Eq. (I-1') in Ref. \cite FaggHanna1959}:
	 * 
	 * \f[
	 * 		W \left( \theta \right) = \sum_\nu A_v \left( L_1, L_1^\prime, j_1, j_2 \right) U_\nu \left( j_2, L_2, L_2^\prime, j_3 \right) ... A_v \left( L_{n-1}, L_{n-1}^\prime, j_n, j_{n-1} \right) P_\nu \left[ \cos \left( \theta \right) \right].
	 * \f]
	 * 
	 * \param theta Polar angle between the direction of the incoming and 
	 * the outgoing photon in radians.
	 * 
	 * \return \f$W \left( \theta \right)\f$
	 */
	double operator()(const double theta) const;

	/**
	 * \brief Return value of the dir-dir correlation at an angle \f$\theta\f$
	 * 
	 * This call operator accepts the angle \f$\varphi\f$ as a second argument, although the 
	 * direction-direction correlation is independent of the azimuthal angle.
	 * Internally, the method calls the single-argument call operator.
	 * 
	 * \param theta Polar angle between the direction of the incoming and 
	 * the outgoing photon in radians.
	 * \param phi Azimuthal angle between the polarization axis of the first photon and the direction of the
	 * outgoing photon in radians. Note that this argument has no influence on the result.
	 * 
	 * \return \f$W \left( \theta \right)\f$
	 */
	double operator()(const double theta, const double) const override {
		return operator()(theta);
	}
	
	/**
	 * \brief Return \f$\nu_\mathrm{max}\f$
	 */
	int get_nu_max() const { return nu_max; };
	/**
	 * \brief Return \f$2 \nu_\mathrm{max}\f$
	 */
	int get_two_nu_max() const { return two_nu_max; };

	double get_normalization_factor() const { return normalization_factor; };

	/**
	 * \brief Calculate products of \f$U_\nu\f$ coefficients for the dir-dir correlation.
	 * 
	 * See also the documentation of W_dir_dir::calculate_expansion_coefficients().
	 * 
	 * \return \f$U_\nu (2) ... U_\nu (n-1)\f$ for \f$ \nu \in \lbrace 0, ..., \nu_\mathrm{max} \rbrace,~ \nu~\mathrm{even} \f$ sorted by increasing values
	 *  of \f$\nu\f$ in a std::vector.
	 */
	vector<double> calculate_expansion_coefficients_Uv() const;

protected:
	/**
	 * \brief Get the maximum value \f$\nu_\mathrm{max}\f$ for which the product of coefficients
	 * \f$A_\nu\f$ and \f$U_\nu\f$ is nonzero.
	 * 
	 * This limits the sum over \f$\nu\f$ in the definition of the direction-direction correlation.
	 * The respective upper limits are given in the documentation of the AvCoefficient and 
	 * UvCoefficient classes.
	 * 
	 * This function calls the function W_dir_dir::calculate_two_nu_max_Av(), 
	 * which calculates the upper limits for the \f$A_\nu\f$ coefficients, and, if there are 
	 * unobserved intermediate transitions, the function W_dir_dir::calculate_two_nu_max_Uv() 
	 * to do the same for the \f$U_\nu\f$ coefficients.
	 * The net limit for \f$\nu\f$ will be the more restrictive of both.
	 * 
	 * Note that the same limit also applies for polarization-direction correlations.
	 * Although they make use of more general coefficients than the \f$A_\nu\f$, they still depend
	 * on the same F coefficients {compare, e.g., Eq. (I-2) and (I-9) in \cite FaggHanna1959}.
	 *
	 * \param ini_to_int Transition from initial state to intermediate state
	 * \param int_state Intermediate state
	 * \param int_to_fin Transition from intermediate state to final state
	 * 
	 * \return \f$2\nu_\mathrm{max}\f$
	 */
	int calculate_two_nu_max() const;

	/**
	 * \brief Get the maximum value \f$\nu_\mathrm{max}\f$ for which the product of coefficients
	 * \f$A_\nu\f$ is nonzero.
	 * 
	 * See also the documentation of W_dir_dir::calculate_two_nu_max().
	 * 
	 * \return \f$2\nu_\mathrm{max}\f$, restriction due to properties of the \f$A_\nu\f$ coefficients
	 */
	int calculate_two_nu_max_Av() const;

	/**
	 * \brief Get the maximum value \f$\nu_\mathrm{max}\f$ for which the product of coefficients
	 * \f$U_\nu\f$ is nonzero.
	 * 
	 * See also the documentation of W_dir_dir::calculate_two_nu_max().
	 * 
	 * \return \f$2\nu_\mathrm{max}\f$, restriction due to properties of the \f$U_\nu\f$ coefficients
	 */
	int calculate_two_nu_max_Uv() const;

	/**
	 * \brief Calculate the set of expansion coefficients for the dir-dir correlation.
	 * 
	 * The sum over \f$\nu\f$ in Eqs. (I-1) and (I-1') of \cite FaggHanna1959 contains products of 
	 * \f$A_\nu\f$ and potentially also \f$U_\nu\f$ coefficients.
	 * 
	 * This function calls the function W_dir_dir::calculate_expansion_coefficients_Av() to 
	 * calculate the products of \f$A_\nu\f$ coefficients from \f$\nu = 0\f$ up to, and including, 
	 * a maximum value of \f$\nu_\mathrm{max}\f$ for a given set of quantum numbers.
	 * If the cascade contains more than two transitions, the corresponding products of 
	 * \f$U_\nu\f$ coefficients for the same values of \f$\nu\f$ are calculated by the 
	 * W_dir_dir::calculate_expansion_coefficients_Av() function.
	 * This function merges the output of the two functions.
	 * 
	 * \return \f$A_\nu (1) A_\nu (2)\f$ (\f$n=3\f$) or \f$A_\nu (1) U_\nu (2) ... A_\nu (n)\f$ (\f$n>3\f$) for \f$ \nu \in \lbrace 0, ..., \nu_\mathrm{max} \rbrace,~ \nu~\mathrm{even} \f$ sorted by increasing values
	 *  of \f$\nu\f$ in a std::vector.
	 */
	vector<double> calculate_expansion_coefficients() const;
	
	/**
	 * \brief Calculate products of \f$A_\nu\f$ coefficients for the dir-dir correlation.
	 * 
	 * See also the documentation of W_dir_dir::calculate_expansion_coefficients().
	 * 
	 * \return \f$A_\nu (1) A_\nu (n)\f$ for \f$ \nu \in \lbrace 0, ..., \nu_\mathrm{max} \rbrace,~ \nu~\mathrm{even} \f$ sorted by increasing values
	 *  of \f$\nu\f$ in a std::vector.
	 */
	vector<double> calculate_expansion_coefficients_Av() const;

	/**
	 * \brief Calculate the normalization factor for the angular correlation.
	 * 
	 * As stated below Eqs. (I-2) and (I-1') in Ref. \cite FaggHanna1959, the \f$A_\nu\f$ 
	 * and \f$U_\nu\f$ coefficients are 
	 * normalized to \f$1 + \delta^2\f$, where \f$\delta\f$ is the multipole mixing ratio that
	 * they contain.
	 * 
	 * In order to normalize the angular correlations to \f$4 \pi\f$, it must be multiplied by:
	 * 
	 * \f[
	 * 		\prod_{i = 1}^{n-1} \frac{1}{1+\delta_i^2},
	 * \f]
	 * 
	 * where \f$\delta_i\f$ is the multipole mixing ratio of the \f$i\f$-th transition.
	 * 
	 * \return \f$\prod_{i} \left( 1+\delta_i^2 \right)^{-1}\f$
	 */
	double calculate_normalization_factor() const;

	const AvCoefficient av_coef; /**< Instance of the AvCoefficient class */
	const UvCoefficient uv_coef; /**< Instance of the UvCoefficient class */
	vector<double> expansion_coefficients; /**< Vector to store expansion coefficients */

};
