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

/**
 * \brief Class for sampling points approximately uniformly on the surface of a sphere
 * 
 * The actual problem of sampling a given number of \f$n\f$ points on the surface of a sphere
 * such that they are uniformly distributed ('Thomson problem' \cite Thomson1904) can only be 
 * solved using numerical optimization methods for arbitrary values of \f$n\f$ \cite Wales2006.
 * 
 * This implementation uses an approximation proposed by Koay2011 \cite Koay2011 that samples 
 * equidistant points on a spiral trajectory 
 * \f$\mathbf{x} \left[ \theta, \varphi \left( \theta \right) \right]\f$ with:
 * {Eq. (3) in \cite Koay2011}
 * 
 * \f[
 *      \varphi \left( \theta \right) = c \theta,
 * \f]
 * 
 * Here, \f$\mathbf{x}\f$ denotes a point in three-dimensional space on the surface of a unit 
 * sphere, which is characterized by the azimuthal (\f$\theta\f$) and polar (\f$\varphi\f$)
 * angles in spherical coordinates.
 * The parameter \f$c\f$ is a positive real number.
 * Note that is was renamed compared to \cite Koay2011 to avoid confusion with the argument of 
 * the elliptic integral that is part of the algorithm.
 * The trajectory, which is defined by the relation between \f$\theta\f$ and \f$\varphi\f$, runs 
 * from one pole of the sphere to the other.
 */
class SpherePointSampler{

public:
    SpherePointSampler() = default;
    ~SpherePointSampler() = default;

    /**
     * \brief Elliptic integral of the second kind \f$E\left( u | m \right)\f$ for arbitrary real parameters
     * 
     * The length of the spiral trajectory on which points are sampled in Ref. \cite Koay2011 
     * can be expressed by an elliptic integral of the second kind.
     * The present implementation uses the definition of Abramowitz and Stegun 
     * \cite AbramowitzStegun1974 (Sec. 17 therein).
     * 
     * The incomplete elliptic integral of the second kind is denoted as \f$E\left(u | m \right)\f$,
     * where \f$u\f$
     * and \f$m\f$ may be arbitrary real numbers (in fact, this is not even the most general 
     * restriction, see, e.g., the discussion below Eq. (17.2.18) in Ref. \cite AbramowitzStegun1974).
     * It is implemented in the GNU Scientific Library (GSL) \cite Galassi2009 for 
     * \f$ 0 \leq m < 1 \f$ and an angle \f$\varphi\f$, which is equal to \f$u\f$, as 
     * Eq. (17.2.2) in Ref. \cite AbramowitzStegun1974 shows:
     * 
     * \f[
     *      \sin \left( \varphi \right) = \mathrm{sn} \left( u \right) \equiv \sin \left( u \right)
     * \f]
     * 
     * The function \f$\mathrm{sn}\f$ denotes a Jacobian elliptic function, whose definition 
     * {Eq. (16.1.5) in Ref. \cite AbramowitzStegun1974} has been inserted in the second equality 
     * of the equation above.
     * The algorithm of Koay \cite Koay2011 requires the evaluation of \f$E \left( u | m \right) \f$
     * for negative values with an absolute value larger than 1, which can be related to the
     * cases that GSL can handle by Eqs. (17.4.16) and (17.4.18) in Ref. \cite AbramowitzStegun1974.
     * 
     * \param u
     * \param m
     * 
     * \return \f$E\left( u | m \right)\f$
     */
    double elliptic_integral_2nd_kind_arbitrary_m(const double u, const double m) const;
};