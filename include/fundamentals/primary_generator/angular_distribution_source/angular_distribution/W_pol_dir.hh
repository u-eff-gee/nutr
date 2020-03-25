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
#include "KappaCoefficient.hh"
#include "State.hh"
#include "Transition.hh"
#include "W_dir_dir.hh"

/**
 * \brief Class for a polarization-direction (pol-dir) correlation
 *
 * Evaluates the angular correlation between two photons in a nuclear reaction of the type 
 * \f$(\vec{\gamma}, \gamma')\f$ in which a nucleus in an initial state with the spin 
 * \f$j_1\f$ absorbs a linearly polarized photon to render it in an excited state 
 * with the spin \f$j\f$, 
 * and emits another photon in a transition to another state with the spin \f$j_2\f$.
 * It is assumed that both the direction and the polarization of the first photon are known, and the 
 * direction of the second photon is measured.
 * 
 * The class uses Eqs. (I-1), (I-2), and (I-4) of Ref. \cite FaggHanna1959 with a similar notation
 * (Note that Ref. \cite FaggHanna1959 assumes that the polarization information is given for the 
 * exit channel.
 * The case of an incoming polarized photon, which is completely analogous, is shown in 
 * Ref. \cite Kneissl1996 and also used here.).
 * It makes use of the dir-dir correlation which is implemented in the class W_dir_dir.
 * See the corresponding files for more information.
 */
class W_pol_dir{
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
	W_pol_dir(const State &ini_state, const Transition &ini_to_int, const State &int_state, const Transition &int_to_fin, const State &fin_state);
	/**
	 * \brief Destructor
	 */
	~W_pol_dir() = default;

	/**
	 * \brief Return value of the pol-dir correlation at angles \f$\theta\f$ and \f$\varphi\f$
	 * 
	 * In Eq. (I-4) of Ref. \cite FaggHanna1959, it is shown that the pol-dir correlation 
	 * can be obtained from the dir-dir correlation by the followin replacement 
	 * (note that the polarization is assumed to be known for the transition \f$j_1 \to j\f$ here, 
	 * instead of \f$j \to j_2\f$):
	 * 
	 * \f[
	 * 		W \left( \theta, \varphi \right) = \sum_\nu A_v \left( L_1, L_1^\prime, j_1, j \right) A_v \left( L_2, L_2^\prime, j_2, j \right) \underbrace{ P_\nu \left[ \cos \left( \theta \right) \right] }_{ P_\nu \left[ \cos \left( \theta \right) \right] + \left( \pm \right)_{L_1^\prime} \cos \left( 2 \varphi \right) \kappa_\nu \left( L_1, L_1^\prime \right) P_\nu^{\left( 2 \right)} \left[ \cos \left( \theta \right) \right]}.
	 * \f]
	 * 
	 * Compared to the dir-dir correlation, the expression contains a term whose sign is 
	 * determined by the electromagnetic charactor of the alternative multipolarity of the first 
	 * transition.
	 * Explicitly, \f$\left( \pm \right)_{L_1^\prime}\f = +1$ if it has electric character, and 
	 * \f$\left( \pm \right)_{L_1^\prime} = -1\f$ if it has magnetic character.
	 * The \f$\cos \left( 2\varphi \right)\f$ term introduces a dependence on the azimuthal angle $\varphi$ with respect to the polarization axis of the first photon.
	 * In Ref. \cite Kneissl1996, the symbol \f$\kappa_\nu \left( L_1, L_1^\prime \right)\f$ is 
	 * called the 'polarization coefficient'.
	 * It is defined in Eq. (I-7) of Ref. \cite FaggHanna1959.
	 * The symbol \f$P_\nu^{\left( 2 \right)}\f$ denotes an associated Legendre polynomial 
	 * of degree \f$\nu\f$ and order 2.
	 * 
	 * \param theta Polar angle between the direction of the incoming and 
	 * the outgoing photon in radians.
	 * \param phi Azimuthal angle between the polarization axis of the first photon and the direction of the
	 * outgoing photon in radians.
	 * 
	 * \return \f$W \left( \theta, \varphi \right)\f$
	 */
	double operator()(const double theta, const double phi);

protected:

	const AvCoefficient av_coef; /**< Instance of the AvCoefficient class */
	const KappaCoefficient kappa_coef; /**< Instance of the KappaCoefficient class */
	const W_dir_dir w_dir_dir; /**< Instance of the W_dir_dir class */

	Transition initial_to_intermediate; /**< Transition from initial state to intermediate state */
	Transition intermediate_to_final; /**< Transition from intermediate state to final state */
	State initial_state; /**< Initial state */
	State intermediate_state; /**< Intermediate state */
	State final_state; /**< Final state */
};
