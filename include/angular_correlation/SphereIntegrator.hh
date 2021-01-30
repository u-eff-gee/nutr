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

#include "SpherePointSampler.hh"

/**
 * \brief Simple integration of a function of 2 variables on a sphere surface
 * 
 * The aim of this class is to calculate an integral of the type
 * 
 * \f[
 *      \int_{\Delta \Omega} f\left( \theta, \varphi \right) \mathrm{d} \Omega
 * \f]
 * 
 * for a function \f$f(\theta, \varphi)\f$ of the polar (\f$\theta\f$) and aimuthal 
 * (\f$\varphi\f$) angles in spherical coordinates.
 * The integration is over a domain \f$\Delta \Omega\f$ on the surface of a sphere.
 * 
 * A simple brute force method is used to perform the integration numerically.
 * It samples \f$n\f$ uniformly distributed points \f$\left( \theta_i, \varphi_i \right)\f$
 * , \f$i \in \left[ 0, n-1 \right]\f$ on the surface of the sphere.
 * The values \f$f\left( theta_i, \varphi_i \right)\f$ are then calculated and assumed to 
 * represent the value of the function on the entire solid-angle element of the size 
 * 
 * \f[
 *      \delta \Omega_i \equiv \delta \Omega = \frac{4 \pi}{n}
 * \f]
 * 
 * which is covered by each point.
 * This can be seen as a crude implementation of the Riemann integral in two dimensions.
 * Consequently, the integral is approximated by a sum:
 * 
 * \f[
 *      \int_{\Delta \Omega} f\left( \theta, \varphi \right) \mathrm{d} \Omega 
 *      \approx \delta \Omega \sum_{i = 0}^{n-1} f \left( \theta_i, \varphi_i \right).
 * \f]
 * 
 * Note that the uniform sampling of points on the surface of a sphere is not trivial.
 * The present implementation uses an approximate method that is implemented in the 
 * SpherePointSampler class.
 * 
 * Note also that the present integration method does not require the weighting factor 
 * \f$\sin\left( \theta \right)\f$ which must be used in spherical coordinates.
 */

class SphereIntegrator{

public:
    SphereIntegrator():
        sph_poi_samp(SpherePointSampler())
    {}

    /**
     * \brief Integrate an arbitary function on a subdomain of a sphere surface
     * 
     * \param f Function of two variables theta and phi.
     * \param n Number of points to be sampled on the sphere surface. Determines the precision 
     * of the integration.
     * \param omega Function which returns true if a given point 
     * \f$\left( \theta, \varphi \right)\f$ is inside the desired domain, and false otherwise.
     * (Default: Function which returns true everywhere, i.e. an integration over the entire
     * surface of the sphere).
     * 
     * \return Value of the integral \f$\int f \mathrm{d} \Omega\f$
     */
    double operator()(double f(double theta, double phi), const unsigned int n, bool is_in_omega(double theta, double phi));

protected:
    const SpherePointSampler sph_poi_samp; /**< Instance of the SpherePointSampler class. */

};