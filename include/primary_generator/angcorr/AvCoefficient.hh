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

#include "FCoefficient.hh"

/**
 * \brief Class for an \f$A_\nu\f$ coefficient.
 * 
 * The \f$A_\nu\f$ coefficients are introduced in Eq. (I-1) of Ref. \cite FaggHanna1959, which is quoted 
 * first before the single constituents are described:
 *
 * \f[
 *	W \left( \theta \right) = \sum_\nu A_\nu \left( 1 \right) A_\nu \left( 2 \right) P_\nu \left[ \cos \left( \theta \right) \right].

 * \f]
 *
 * This equation describes the angular correlation \f$W \left( \theta \right)\f$ between an
 * incoming particle and an outgoing particle in a nuclear reaction.
 * It is assumed that only directions of the incoming and the outgoing particles are measured 
 * {a 'direction-direction' (dir-dir) correlation in the nomenclature of Ref. \cite FaggHanna1959},
 * which differ by the polar angle \f$\theta\f$ that is defined with respect to the direction of 
 * the incoming particle.
 * The angular dependence is incorporated in ordinary legendre polynomials 
 * \f$P_\nu \left( \theta \right)\f$ of the order \f$\nu\f$.
 * The coefficients \f$A_\nu \left( n \right)\f$ encode the properties of the three nuclear 
 * states and the two radiative transitions \f$n=1\f$ and \f$n=2\f$ which are involved in 
 * the reaction.
 * In principle, the equation above allows for an arbitrarily complicated combination of
 * angular momentum quantum numbers ('spins') \f$j\f$ of the intermediate excited state of 
 * the nucleus and multipolarities \f$L\f$ of the radiative transitions.
 * Accordingly, the argument \f$n\f$ of \f$A_\nu \left( n \right)\f$ may stand for an arbitrary 
 * number of parameters.
 * If only a single angular momentum quantum number is allowed for the intermediate state, and 
 * only the two dominant multipoles \f$L\f$ and \f$L^\prime\f$ are considered, the following
 * expression for the coefficients is obtained {Eq. (I-2) of Ref. \cite FaggHanna1959}:
 *
 * \f[
 *	A_\nu \left( L_n, L_n^\prime, j_n, j, \delta_n \right) =
 *	F_\nu \left( L_n, L_n, j_n, j \right)
 *	+ 2 \delta_n F_\nu \left( L_n, L_n^\prime, j_n, j \right)
 *	+ \delta_n^2 F_\nu \left( L_n^\prime, L_n^\prime, j_n, j \right).
 * \f]
 *
 * The coefficients \f$A_\nu\f$ are combinations of F coefficients that depend on the 
 * spin \f$j\f$ of the intermediate state of the nucleus, the spin of the initial (\f$n=1\f$) or 
 * the final state (\f$n=2\f$) \f$j_n\f$ of the nucleus, and the primariy and secondary
 * multipolarities \f$L_n\f$ and \f$L_n^\prime\f$ of the respective transition \f$^1\f$.
 * For each transition, the ratio of the intensities of both multipolarities is quantified 
 * by a multipole mixing ratio \f$\delta_n\f$.
 * Note that several definitions of the multipole mixing ratio coexist in the literature which 
 * differ by phase factors of the type \f$\left( -1 \right)^{L_n - L_n^\prime}\f$.
 * For an early account of the problem, see for example the discussion in Sec. V.A of a review 
 * article by Rose and Brink \cite RoseBrink1967
 * or the corresponding section in the 'Procedures Manual for the Evaluated Nuclear Structure Data 
 * File' \cite ENSDF1987.
 * In principle, any phase convention can be used with the present code, if it is applied 
 * consistently.
 *
 * The selection rules for the F coefficients eventually terminate the sum over the parameter 
 * \f$\nu\f$ in the definition of \f$W\left( \theta \right)\f$.
 * The maximum value \f$\nu_\mathrm{max}\f$ for which a single coefficient \f$A_\nu\f$ does not
 * vanish can be found by considering the selection rules of the F coefficients, and,
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
 * 		0 \leq \nu \leq \min \lbrace 2j, \max \left( 2 L_1, 2 L_1^\prime \right), \max \left( 2 L_2, 2 L_2^\prime \right) \rbrace.
 * \f]
 * 
 * Note that no assumption about the relative magnitude of \f$L_n\f$ and \f$L_n^\prime\f$
 * was made in this derivation.
 * Many authors also give simplified expressions for \f$L_n^\prime = L_n^\prime + 1\f$,
 * which represents a relatively common case, but the present code can take any values for \f$L_n\f$ and \f$L_n^\prime\f$.
 *
 * In this context, note also that the authors of Ref. \cite Kneissl1996 restrict the summation to 
 * \f$\nu = 0, 2, 4\f$ for photonic transitions with \f$L \leq 2\f$ of even-even nuclei.
 *
 * \f$^1\f$ To the author, the notation in Sec. B of Ref. \cite FaggHanna1959 appeared confusing 
 * at first, because the parameters of the \f$A_\nu \left( n \right)\f$ are first introduced 
 * as '\f$1 \to L_1 L_1^\prime j j^\prime\f$' and '\f$2 \to L_2 L_2^\prime j j^\prime\f$' below 
 * Eq. (I-1).
 * Later, above Eq. (I-2), \f$j\f$ and \f$j^\prime\f$ are said to be equal, but then the 
 * notation with an index \f$n\f$ is introduced in Eq. (I-2).
 * Although the paragraph following Eq. (I-2) seems to clarify this, the meaning of the spin
 * variables was cross checked with the NRF literature {for example Eqs. (8) and (9) 
 * in \cite Kneissl1996}.
 */

class AvCoefficient{
public:
	AvCoefficient();	
	~AvCoefficient() = default;

	/**
	 * \brief Return value of a specific \f$A_\nu\f$ coefficient.
	 * 
	 * The order of arguments is the same as for the F coefficients for all that \f$A_\nu\f$
	 * and F have in common.
	 * The multipole mixing ratio was added as the last parameter.
	 *
	 * \param two_nu \f$2 \nu\f$
	 * \param two_L Primary multipolarity \f$2 L\f$
	 * \param two_Lp Secondary multipolarity \f$2 L^\prime\f$
	 * \param two_jn Angular momentum quantum number \f$2 j_n\f$ of the initial or final state
	 * 	of a transition 
	 * \param two_j Angular momentum quantum number \f$2 j\f$ of the intermediate state
	 * 	of a transition 
	 * \param delta Multipole mixing ratio \f$\delta\f$
	 *
	 * \return \f$A_\nu \left( L, L^\prime, j_n, j, \delta_n \right)\f$
	 */
	double operator()(const int two_nu, const int two_L, const int two_Lp, const int two_jn, const int two_j, const double delta) const;

protected:
	const FCoefficient f_coef; /**< Instance of the F coefficient class */
};
