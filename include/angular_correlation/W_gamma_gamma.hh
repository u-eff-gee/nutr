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

#include <vector>
#include <utility>

#include "State.hh"
#include "Transition.hh"

using std::pair;
using std::vector;

/**
 * \brief Abstract class for angular correlations of two photons.
 * 
 * Base class for angular correlations of two photons, which defines the general API.
 * A gamma-gamma angular correlation \f$W \left( \theta, \varphi \right)\f$is defined by an 
 * oriented initial state and a set of cascade steps, which are pairs of a transition and a state 
 * to which this transition leads.
 * The angular correlation is a function of two variables, the polar angle \f$\theta\f$ and
 * the azimuthal angle \f$\varphi\f$ in spherical coordinates.
 */
class W_gamma_gamma{

public:
	/**
	 * \brief Constructor
	 * 
	 * \param ini_sta Oriented intial state.
	 * \param cas_ste Steps of the cascade which follow the exciation of the initial state.
	 * Each step is a pair of a transition and the state which is populated by that transition.
	 */
    W_gamma_gamma(const State &ini_sta, const vector<pair<Transition, State>> cas_ste):
        initial_state(ini_sta), cascade_steps(cas_ste), n_cascade_steps(cas_ste.size())
    {}
	/**
	 * \brief Destructor
	 */
	~W_gamma_gamma() = default;

	/**
	 * \brief Call operator of the gamma-gamma angular correlation
	 * 
	 * Returns the value of the angular correlation at a polar angle \f$\theta\f$ and an azimuthal
	 * angle \f$\varphi\f$ in spherical coordinates.
	 * 
     * \param theta Polar angle in spherical coordinates in radians 
     * (\f$\theta \in \left[ 0, \pi \right]\f$).
     * \param phi Azimuthal angle in spherical coordinates in radians 
     * (\f$\varphi \in \left[ 0, 2 \pi \right]\f$).
	 * 
     * \return \f$W_{\gamma \gamma} \left( \theta, \varphi \right)\f$
	 */
	virtual double operator()(const double theta, const double phi) const = 0;
	
	/**
	 * \brief Return an upper limit for possible values of the gamma-gamma angular correlation.
	 * 
	 * Some applications, for example the rejection-sampling (or 'accept-reject') algorithm 
	 * (see, e.g. Sec. 2.3 in \cite RobertCasella1999), which can be used to sample random 
	 * directions that are distributed according to a given angular correlation, require an 
	 * expression, or at least an estimate, for the maximum absolute value of 
	 * \f$W \left( \theta, \varphi \right)\f$, i.e.:
	 * 
	 * \f[
	 * 		\mathrm{max}_{\theta \in \left[ 0, \pi \right], \varphi \in \left[ 0, 2\pi \right]} | W \left( \theta, \varphi \right) |.
	 * \f]
	 * 
	 * If a useful upper limit estimate exists for a given angular correlation, this function will
	 * return it.
	 * If no useful upper limit exists, or the absolute value of \f$W\f$ does not have a limit, this
	 * function returns a negative number.
	 * 
	 * \return \f$\mathrm{max}_{\theta \in \left[ 0, \pi \right], \varphi \in \left[ 0, 2\pi \right]} 
	 * | W \left( \theta, \varphi \right) | \f$, or an upper limit for this quantity.
	 * If no useful upper limit can be given or if there is no limit, a negative number is returned.
	 */
	virtual double get_upper_limit() const = 0;

protected:
    State initial_state; /**< Initial state */
	/** 
	 * Steps of the gamma-ray cascade following an excitation.
	 * Each step consists of an electromagnetic transition and a state which is populated by 
	 * that transition.
	 */
    vector<pair<Transition, State>> cascade_steps;
	
	double normalization_factor; /**< Normalization factor for the angular distribution */
    const size_t n_cascade_steps; /**< Number of transitions in the cascade. */
	int two_nu_max; /**< Maximum value of \f$2 \nu\f$ for which the coefficients do not vanish */
	int nu_max; /**< Maximum value of \f$\nu\f$ for which the coefficients do not vanish */

};