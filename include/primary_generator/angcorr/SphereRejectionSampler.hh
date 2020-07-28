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

#include <functional>
#include <random>
#include <utility>

using std::function;
using std::mt19937;
using std::pair;
using std::tuple;
using std::uniform_real_distribution;

/**
 * \brief Sample from a probability distribution in spherical coordinates using rejection sampling.
 * 
 * Given a probability distribution 
 * 
 * \f[
 * W\left( \Omega \right) \mathrm{d}\Omega = W \left( \theta, \varphi\right) \sin \left( \theta \right) \mathrm{d} \theta \mathrm{d} \varphi,
 * \f]
 * 
 * which determines the probability of finding a vector in the solid angle element 
 * \f$\mathrm{d}\Omega\f$ (expressed as 
 * \f$\sin \left( \theta \right) \mathrm{d} \theta \mathrm{d} \varphi\f$ in spherical coordinates, 
 * with the polar angle \f$\theta\f$ and the aximuthal angle \f$\varphi\f$)
 * this class samples random tuples \f$\left( \theta, \varphi \right)\f$ distributed according to \f$W\f$.
 * 
 * The sampling algorithm used here is 'rejection sampling' (see, e.g. Sec. 2.3 in Ref.
 * \cite RobertCasella1999).
 * It requires an upper limit \f$W_\mathrm{max}\f$ for the maximum value of the distribution:
 * 
 * \f[
 *      W_\mathrm{max} \geq \mathrm{max}_{\theta \in \left[ 0, \pi \right], \varphi \in \left[ 0, 2\pi \right]} W \left( \theta, \varphi \right).
 * \f]
 * 
 * It starts by sampling a point \f$\left( \theta_\mathrm{rand}, \varphi_\mathrm{rand} \right)\f$ 
 * from a uniform distribution on a sphere surface (see, e.g. Ref. \cite Weisstein20202):
 * 
 * \f[
 *      \theta_\mathrm{rand} = \mathrm{arccos} \left( 2u - 1 \right)
 * \f]
 * \f[
 *      \varphi_\mathrm{rand} = 2 v \pi.
 * \f]
 * 
 * Here, \f$u\f$ and \f$v\f$ denote independent uniform random numbers in the range \f$\left[ 0, 1 \right]\f$.
 * After that, a uniform random number \f$W_\mathrm{rand}\f$ in the range 
 * \f$\left[ 0, W_\mathrm{max} \right]\f$ is sampled.
 * If
 * 
 * \f[
 *      W_\mathrm{rand} \leq W \left( \theta_\mathrm{rand}, \varphi_\mathrm{rand} \right),
 * \f]
 * 
 * then the vector \f$\left( \theta_\mathrm{rand}, \varphi_\mathrm{rand} \right)\f$ is accepted.
 * If not, the vector is rejected and the algorithm start from the beginning by sampling a new 
 * point on the sphere surface.
 * Note that the last inequality ensures that the probability of sampling a vector 
 * \f$\left( \theta, \varphi \right)\f$ is proportional to \f$W \left( \theta, \varphi \right)\f$.
 * This algorithm obviously works best if the probability of a vector being rejected is small, 
 * i.e. if \f$W_\mathrm{max}\f$ is a good approximation of the real maximum of \f$W\f$, and if the
 * distribution is not too different from a uniform distribution 
 * \f$W = \left( 4 \pi \right)^{-1}\f$.
 * Note that rejection sampling does not require the distribution to be normalized.
 * 
 * In principle, rejection sampling proceeds in an infinite loop until a valid vector has been 
 * found.
 * Here, the loop is limited to a maximum number of trial vectors \f$N_\mathrm{max}\f$.
 * If this maximum number is reached, the point \f$\left( 0, 0 \right)\f$ is returned.
 * 
 * As a measure of the efficiency of the algorithm for a given distribution, define the efficiency
 * 
 * \f[
 *      \epsilon = \frac{1}{\langle N \rangle},
 * \f]
 * 
 * which is the inverse of the average number of trial vectors \f$\langle N \rangle\f$ that have 
 * to be sampled before a vector is accepted.
 */
class SphereRejectionSampler{

public:
    /**
     * \brief Constructor
     * 
     * \param dis \f$W \left( \theta, \varphi \right)\f$, probability distribution in spherical 
     * coordinates.
     * \param dis_max \f$W_\mathrm{max}\f$, upper limit for the maximum of the 
     * probability distribution.
     * \param seed Random number seed.
     * \param max_tri Maximum number of sampled points 
     * \f$\left( \theta_\mathrm{rand}, \varphi_\mathrm{rand} \right)\f$
     * before the algorithm terminates without success and returns \f$\left( 0, 0 \right)\f$.
     */
    SphereRejectionSampler(function<double(const double, const double)> dis, const double dis_max, const int seed, const unsigned int max_tri = 1000);

    /**
     * \brief Sample a random vector from probability distribution and record the number of tries.
     * 
     * \return Triple \f$\left( N, \theta_\mathrm{rand}, \varphi_\mathrm{rand}\right)\f$ which 
     * contains an accepted vector \f$\left( \theta_\mathrm{rand}, \varphi_\mathrm{rand}\right)\f$
     * and the number of tries \f$N\f$ which were needed to find this vector.
     * Returns \f$\left( N_\mathrm{max}, 0, 0 \right)\f$ if the maximum number of trials \f$N_\mathrm{max}\f$ is
     * reached by the algorithm and no random vector was accepted.
     */
    tuple<unsigned int, double, double> sample();

    /**
     * \brief Sample a random vector from probability distribution.
     * 
     * \return Accepted vector \f$\left( \theta_\mathrm{rand}, \varphi_\mathrm{rand}\right)\f$.
     * Returns \f$\left( 0, 0 \right)\f$ if the maximum number of trials \f$N_\mathrm{max}\f$ is
     * reached by the algorithm and no random vector was accepted.
     */
    pair<double, double> operator()();

    /**
     * \brief Estimate the efficiency of rejection sampling for the given distribution.
     * 
     * The efficiency \f$\epsilon\f$ is estimated by sampling \f$n\f$ vectors from the 
     * distribution and calculating the average number of trials \f$\langle N \rangle\f$ for this set.
     * 
     * \param n_tries \f$n\f$, number of sampled vectors.
     * 
     * \return Estimate for \f$\epsilon\f$ from the \f$n\f$ samples.
     */
    double estimate_efficiency(const unsigned int n_tries);

protected:
    /**
     * \brief Sample polar angle of a uniformly randomly distributed point on a sphere surface.
     * 
     * \return \f$\theta_\mathrm{rand}\f$, random polar angle.
     */
    double sample_theta();

    /**
     * \brief Sample azimuthal angle of a uniformly randomly distributed point on a sphere surface.
     * 
     * \return \f$\varphi_\mathrm{rand}\f$, random azimuthal angle.
     */
    double sample_phi();

    /**
     * \brief Sample uniformly randomly distributed point on a sphere surface.
     * 
     * \return \f$\left( \theta_\mathrm{rand}, \varphi_\mathrm{rand} \right)\f$, random point on
     * sphere surface.
     */
    pair<double, double> sample_theta_phi();

    function<double(const double, const double)> distribution; /**< \f$W \left( \theta, \varphi \right)\f$, (unnormalized) probability distribution. */
    const double distribution_maximum; /**< \f$W_\mathrm{max}\f$, maximum of probability distribution. */
    const unsigned int max_tries; /**< \f$N_\mathrm{max}\f$, maximum number of tries to find a random vector. */

    mt19937 random_engine; /**< Deterministic random number engine. */
    uniform_real_distribution<double> uniform_random; /**< Uniform distribution from which all random numbers are derived here. */
};