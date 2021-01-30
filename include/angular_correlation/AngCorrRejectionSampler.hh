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

#include <memory>

using std::unique_ptr;

#include "SphereRejectionSampler.hh"
#include "W_gamma_gamma.hh"

/**
 * \brief Sample from a gamma-ray angular correlation in spherical coordinates.
 * 
 * Compared to its base class, SphereRejectionSampler, this class provides a member variable to
 * store a W_gamma_gamma object that acts as the probability distribution \f$W\f$.
 * Although SphereRejectionSampler already accepts an arbitrary function of \f$\theta\f$ and
 * \f$\varphi\f$, a function object of the class W_gamma_gamma cannot be passed this way.
 * Therefore, the present class was derived.
 * For more information, see the base class.
 */
class AngCorrRejectionSampler: public SphereRejectionSampler{

public:
    /**
     * \brief Constructor
     * 
     * In contrast to the base class, the upper limit \f$W_\mathrm{max}\f$ does not have to be 
     * provided explicitly. 
     * The member function W_gamma_gamma::get_upper_limit() is called instead.
     * 
     * \param w \f$W \left( \theta, \varphi \right)\f$, angular correlation
     * \param seed Random number seed.
     * \param max_tri Maximum number of sampled points 
     * \f$\left( \theta_\mathrm{rand}, \varphi_\mathrm{rand} \right)\f$
     * before the algorithm terminates without success and returns \f$\left( 0, 0 \right)\f$.
     */
    AngCorrRejectionSampler(W_gamma_gamma* w, const int seed, const unsigned int max_tri = 1000);

    /**
     * \brief Sample a random vector from probability distribution and record the number of tries.
     * 
     * \return Triple \f$\left( N, \theta_\mathrm{rand}, \varphi_\mathrm{rand}\right)\f$ which 
     * contains an accepted vector \f$\left( \theta_\mathrm{rand}, \varphi_\mathrm{rand}\right)\f$
     * and the number of tries \f$N\f$ which were needed to find this vector.
     * Returns \f$\left( N_\mathrm{max}, 0, 0 \right)\f$ if the maximum number of trials \f$N_\mathrm{max}\f$ is
     * reached by the algorithm and no random vector was accepted.
     */
    tuple<unsigned int, double, double> sample() override final;

protected:
    unique_ptr<W_gamma_gamma> w_gamma_gamma; /**< Gamma-ray angular correlation */
};