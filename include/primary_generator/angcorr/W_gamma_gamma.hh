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
#include <utility>

#include "State.hh"
#include "Transition.hh"

using std::pair;
using std::vector;

/**
 * \brief Abstract class for angular correlations of two photons.
 * 
 * Base class for angular correlations of two photons, which defines the general API.
 * A gamma-gamma angular correlation is defined by an oriented initial state and a set of 
 * cascade steps, which are pairs of a transition and a state to which this transition leads.
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

	virtual double operator()(const double theta, const double phi) const = 0;

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