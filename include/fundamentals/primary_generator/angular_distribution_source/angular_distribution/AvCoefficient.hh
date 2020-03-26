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
 *	W \left( \theta \right) = \sum_\nu A_\nu \left( 1 \right) A_\nu \left( 2 \right) P_\nu \left( \cos \theta \right).

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
 * If only a single angular momentum quantum number is allowed for the intermediate state, and 
 * only the two dominant multipoles \f$L\f$ and \f$L^\prime\f$ are considered, the following
 * expression for the coefficients is obtained {Eq. (I-2) of Ref. \cite FaggHanna1959}:
 *
 * \f[
 *	A_\nu (n) \left( L_n, L_n^\prime, j_n, j, \delta_n \right) =
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
 *
 * The selection rules for the F coefficients eventually terminate the sum over the parameter 
 * \f$\nu\f$ in the definition of \f$W\left( \theta \right)\f$.
 * Explicitly, the selection rules for the Wigner symbols in the F coefficient,
 *
 * \f[
 *	0 \leq k \leq 2j,
 * \f]
 *
 * \f[
 *	k \leq L + L^\prime,
 * \f]
 *
 * and 
 *
 * \f[
 *	2 j + k ~~~ \mathrm{even}
 * \f]
 *
 * are effective.
 * For this reason, the authors of Ref. \cite Kneissl1996 restrict the summation to 
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
