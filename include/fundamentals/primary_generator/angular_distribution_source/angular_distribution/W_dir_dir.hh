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

#include <vector>

#include "AvCoefficient.hh"
#include "State.hh"
#include "Transition.hh"

using std::vector;

/**
 * \brief Class for a direction-direction (dir-dir) correlation
 *
 * Evaluates the angular correlation between two photons in a nuclear reaction of the type 
 * \f$(\gamma, \gamma')\f$ in which a nucleus in an initial state with the spin 
 * \f$j_1\f$ absorbs a photon to render it in an excited state with the spin \f$j\f$, 
 * and emits another photon in a transition to another state with the spin \f$j_2\f$.
 * It is assumed that only the direction of the two photons in the process is known,
 * but no polarization information.
 * In particular, this means that the dir-dir correlation does not depend on the parity
 * quantum number of the involved nuclear states, which translates into an independence of the
 * azimuthal angle of the emitted photon.
 * 
 * The class uses Eqs. (I-1) and (I-2) of Ref. \cite FaggHanna1959 with a similar notation.
 * Note that Eq. (I-2) already includes the assumption that only two multipoles contribute
 * to any of the two transitions, and that the spin of the intermediate state is known.
 *
 */
class W_dir_dir{
public:
	/**
	 * \brief Constructor
	 * 
	 * \param ini_state Intial state
	 * \param ini_to_int Transition from initial state to intermediate state
	 * \param int_state Intermediate state
	 * \param int_to_fin Transition from intermediate state to final state
	 * \param fin_state Final state
	 */
	W_dir_dir(const State &ini_state, const Transition &ini_to_int, const State &int_state, const Transition &int_to_fin, const State &fin_state);
	/**
	 * \brief Destructor
	 */
	~W_dir_dir() = default;

	/**
	 * \brief Return value of the dir-dir correlation at an angle \f$\theta\f$
	 * 
	 * It is given by the following expression {Eqs. (I-1) and (I-2) in Ref. \cite FaggHanna1959}:
	 * 
	 * \f[
	 * 		W \left( \theta, \varphi \right) = \sum_\nu A_v \left( L_1, L_1^\prime, j_1, j \right) A_v \left( L_2, L_2^\prime, j_2, j \right) P_\nu \left[ \cos \left( \theta \right) \right].
	 * \f]
	 * 
	 * Here, a level sequence \f$j_1 \to j \to \j_2\f$ is assumed.
	 * The first transition proceeds via the multipolarities \f$L_1\f$ and \f$L_1^\prime\f$,
	 * while the second has the multipolarities \f$L_2\f$ and \f$L_2^\prime\f$.
	 * The symbol \f$P_v\f$ denotes a Legendre polynomial of the degree \f$\nu\f$.
	 * It incorporates the dependence on the polar angle \f$\theta\f$, while the dependence on the
	 * nuclear properties is encoded in the \f$A_\nu\f$ coefficients.
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
	 * It is intended for easy switching between the two classes without having to change a lot
	 * of code.
	 * Internally, the method calls the single-argument call operator.
	 * 
	 * \param theta Polar angle between the direction of the incoming and 
	 * the outgoing photon in radians.
	 * \param phi Azimuthal angle between the polarization axis of the first photon and the direction of the
	 * outgoing photon in radians. Note that this argument has no influence on the result.
	 * 
	 * \return \f$W \left( \theta \right)\f$
	 */
	double operator()(const double theta, const double phi) const {
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

protected:
	/**
	 * \brief Get the maximum value \f$\nu_\mathrm{max}\f$ for which the product of coefficients
	 * \f$A_\nu \left( 1 \right) A_\nu \left( 2 \right)\f$ is nonzero.
	 * This limits the sum over \f$\nu\f$ in the definition of the direction-direction correlation.
	 *
	 * The maximum value \f$\nu_\mathrm{max}\f$ for which a single coefficient \f$A_\nu\f$ does not
	 * vanish is obtained here by considering the selection rules of the F coefficients, and,
	 * more specifically, the selection rules of the constituting Clebsch-Gordan and Racah
	 * coefficients.
	 *
	 * Note that the assumption of two multipole orders and a known spin of the
	 * intermediate state lead to the restriction {mentioned above Eq. (I-2) in 
	 * Ref. \cite FaggHanna1959}:
	 *
	 * \f[
	 *	\nu \% 2 = 0,
	 * \f]
	 *
	 * i.e. \f$\nu\f$ can only be an even integer number.
	 *
	 * The following Clebsch-Gordan coefficients appear in the definition of a single 
	 * \f$A_\nu\f$ coefficient {see Eq. (I-2) in \cite FaggHanna1959 and Eq. (1) in 
	 * \cite FerentzRosenzweig1955}:
	 *
	 * \f[
	 * 	\left(
	 *		\begin{array}{ccc}
	 *			L_n & L_n & \nu \\
	 *			1 & -1 & 0
	 *		\end{array}
	 *	\right)
	 * \f]
	 * \f[
	 * 	\left(
	 *		\begin{array}{ccc}
	 *			L_n & L_n^\prime & \nu \\
	 *			1 & -1 & 0
	 *		\end{array}
	 *	\right)
	 * \f]
	 * \f[
	 * 	\left(
	 *		\begin{array}{ccc}
	 *			L_n^\prime & L_n^\prime & \nu \\
	 *			1 & -1 & 0
	 *		\end{array}
	 *	\right)
	 * \f]
	 *
	 * The selection rules for the Clebsch-Gordan coefficients above that involve \f$\nu\f$ are
	 * {Sec. C.I.3 in \cite Messiah19622}:
	 *
	 * \f[
	 *	\left| L_n - L_n \right| = 0 \leq \nu \leq 2 L_n
	 * \f]
	 * \f[
	 *	\left| L_n - L_n^\prime \right| \leq \nu \leq L_n + L_n^\prime
	 * \f]
	 * \f[
	 *	\left| L_n^\prime - L_n^\prime \right| = 0 \leq \nu \leq 2 L_n^\prime.
	 * \f]
	 *
	 * In principle, there is also the requirement that \f$ M \leq J\f$ 
	 * (in the notation of Ref. \cite Messiah19622), which is always fulfilled here since
	 * \f$M = 0\f$.
	 * From the selection rules, it can be concluded that:
	 *
	 * \f[
	 *	0 \leq \nu \leq \max \left( 2 L_n, 2 L_n^\prime \right).
	 * \f]
	 *
	 * In addition, the Wigner-6j symbols (Racah coefficients without the phase factor)
	 *
	 * \f[
	 *	\left\lbrace
	 *		\begin{array}{ccc}
	 *			j & j & \nu \\
	 *			L_n & L_n & j_n
	 *		\end{array}
	 *	\right\rbrace
	 * \f]
	 * \f[
	 *	\left\lbrace
	 *		\begin{array}{ccc}
	 *			j & j & \nu \\
	 *			L_n^\prime & L_n & j_n
	 *		\end{array}
	 *	\right\rbrace
	 * \f]
	 * \f[
	 *	\left\lbrace
	 *		\begin{array}{ccc}
	 *			j & j & \nu \\
	 *			L_n^\prime & L_n^\prime & j_n
	 *		\end{array}
	 *	\right\rbrace,
	 * \f]
	 *
	 * which appear in a coefficient \f$A_\nu\f$ {see Eq. (I-2) in \cite FaggHanna1959 and 
	 * Eq. (1) in \cite FerentzRosenzweig1955} provide the following selection rules 
	 * for \f$\nu\f$ {Sec. C.II.7 in \cite Messiah19622}:
	 * 
	 * \f[
	 *	\left| j - j \right| = 0 \leq \nu \leq 2j
	 * \f]
	 * \f[
	 *	2j + \nu ~~~~ \mathrm{integer}
	 * \f]
	 * \f[
	 *	2L_n + \nu ~~~~ \mathrm{integer}
	 * \f]
	 * \f[
	 *	L_n + L_n^\prime + \nu ~~~~ \mathrm{integer}
	 * \f]
	 * \f[
	 *	2L_n^\prime + \nu ~~~~ \mathrm{integer}.
	 * \f]
	 *
	 * They also impose the same triangle inequalities as the Clebsch-Gordan 
	 * coefficients via the relation between \f$J_1\f$, \f$J_2\f$, 
	 * and \f$J_3\f$ (in the notation of Ref. \cite Messiah19622).
	 * All conditions of the type '\f$... + \nu\f$ is integer' confirm that 
	 * \f$\nu\f$ is an integer, since the multipolarities \f$L_n\f$ and \f$L_n^\prime\f$, 
	 * as well as the quantity \f$2j\f$ are integers.
	 *
	 * In summary, the coefficients \f$A_\nu\f$ will be nonzero if
	 *
	 * \f[
	 *	\nu \% 2 = 0,
	 * \f]
	 * \f[
	 *	0 \leq \nu \leq \min \left[ 2 j, \max \left( 2 L_n, 2 L_n^\prime \right) \right],
	 * \f]
	 *
	 * since at least one of the three F coefficients will have a nonzero value.
	 * 
	 * Each term in the summation over \f$\nu\f$ in the expression for
	 * \f$W\left( \theta \right)\f$ includes a product of \f$A_\nu\f$ coefficients 
	 * {Eq. (I-2) in Ref. \cite FaggHanna1959}.
	 * They are proportional to 9 unique products of F coefficients 
	 * {see, e.g., Eqs. (I-1) and (I-2) in \cite FaggHanna1959} of which the first three are:
	 * 
	 * \f[
	 * 		F_\nu \left( L_1, L_1, j_1, j \right) F_\nu \left( L_2, L_2, j_2, j \right), ~~~~ \nu \leq \min \left[ 2j, \min \left( 2L_1, 2L_2 \right) \right]
	 * \f]
	 * \f[
	 * 		F_\nu \left( L_1, L_1, j_1, j \right) F_\nu \left( L_2, L_2^\prime, j_2, j \right), ~~~~ \nu \leq \min \left[ 2j, \min \left( 2L_1, 2L_2 + 2L_2^\prime \right) \right]
	 * \f]
	 * \f[
	 * 		F_\nu \left( L_1, L_1, j_1, j \right) F_\nu \left( L_2^\prime, L_2^\prime, j_2, j \right), ~~~~ \nu \leq \min \left[ 2j, \min \left( 2L_1, 2L_2^\prime \right) \right]
	 * \f]
	 * \f[
	 * 		...
	 * \f]
	 * 
	 * In the equations above, the condition for each product to be nonzero is shown on the right,
	 * which follows from the properties of the Clebsch-Gordan - and Wigner-6j symbols.
	 * The terms which vanish last as \f$\nu\f$ increases are the ones that contain the maximum 
	 * values for the multipole order \f$L_n\f$ of a given transition, if the sum is not 
	 * terminated earlier by the condition which depends on \f$j\f$.
	 * As it is also stated after Eq. (40) (an expression for the direction-direction correlation) 
	 * in the review article by Biedenharn and Rose \cite BiedenharnRose1953 {the author found 
	 * that such statements are sometimes wrong in the literature, like Eq. (13) in Ref. 
	 * \cite FerentzRosenzweig1955, for example, or not given at all.}
	 * the non-vanishing terms are then given by:
	 * 
	 * \f[
	 * 		0 \leq \nu \leq \min \lbrace 2j, \max \left( 2 L_1, 2 L_1^\prime \right), \max \left( 2 L_2, 2 L_2^\prime \right) \rbrace
	 * \f]
	 * 
	 * Note that the same argument applies for polarization-direction correlations.
	 * Although they make use of more general coefficients than the \f$A_\nu\f$, they still depend
	 * on the same F coefficients {compare, e.g., Eq. (I-2) and (I-9) in \cite FaggHanna1959}.
	 * Note also that no assumption about the relative magnitude of \f$L_n\f$ and \f$L_n^\prime\f$
	 * was made in this derivation.
	 * Many authors also give simplified expressions for \f$L_n^\prime = L_n^\prime + 1\f$,
	 * which represents a relatively common case, but the present code can take any values for \f$L_n\f$ and \f$L_n^\prime\f$.
	 *
	 * \param ini_to_int Transition from initial state to intermediate state
	 * \param int_state Intermediate state
	 * \param int_to_fin Transition from intermediate state to final state
	 * 
	 * \return \f$2\nu_\mathrm{max}\f$
	 */
	int get_two_nu_max(const Transition &ini_to_int, const State &int_state, const Transition &int_to_fin) const;

	/**
	 * \brief Calculate set of products of \f$A_v\f$ coefficients
	 * 
	 * The sum over \f$\nu\f$ in Eq. (I-1) of \cite FaggHanna1959 contains products of 
	 * \f$A_v\f$ coefficients of the form:
	 * 
	 * \f[
	 * 		A_v \left( L_1, L_1^\prime, j_1, j, \delta_1 \right) 
	 * 		A_v \left( L_2, L_2^\prime, j_2, j, \delta_2 \right).
	 * \f]
	 * 
	 * This function calculates all products from \f$\nu = 0\f$ up to, and including, a maximum 
	 * value of \f$\nu_\mathrm{max}\f$ for a given set of quantum numbers, and returns them 
	 * as a vector.
	 * 
	 * \param two_nu_max Maximum value of \f$2 \nu\f$
	 * \param ini_state Intial state
	 * \param ini_to_int Transition from initial state to intermediate state
	 * \param int_state Intermediate state
	 * \param int_to_fin Transition from intermediate state to final state
	 * \param fin_state Final state
	 * 
	 * \return \f$A_v (L_1, L_1^\prime, j_1, j, \delta_1) A_v (L_2, L_2^\prime, j_2, j, \delta_2) ~~,
	 * ~~ \nu \in \lbrace 0, ..., \nu_\mathrm{max} \rbrace ~~,~~ \nu~\mathrm{even} \f$ sorted by increasing values
	 *  of \f$\nu\f$ in a std::vector.
	 */
	vector<double> get_av_products(const int two_nu_max, const State &ini_state, const Transition &ini_to_int, const State &int_state, const Transition &int_to_fin, const State &fin_state) const;

	const AvCoefficient av_coef; /**< Instance of the AvCoefficient class */
	vector<double> av_prod_cache; /**< Vector to store \f$A_v\f$ coefficients */

	Transition initial_to_intermediate; /**< Transition from initial state to intermediate state */
	Transition intermediate_to_final; /**< Transition from intermediate state to final state */
	State initial_state; /**< Initial state */
	State intermediate_state; /**< Intermediate state */
	State final_state; /**< Final state */

	int nu_max; /**< Maximum value of \f$\nu\f$ for which the coefficients do not vanish */
	int two_nu_max; /**< Maximum value of \f$2 \nu\f$ for which the coefficients do not vanish */
};
