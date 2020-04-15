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

#include <array>
#include <vector>

using std::array;
using std::vector;

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
 * Note that it was renamed compared to \cite Koay2011 to avoid confusion with the argument of 
 * the elliptic integral that is part of the algorithm.
 * The trajectory, which is defined by the relation between \f$\theta\f$ and \f$\varphi\f$, runs 
 * from the pole of the sphere at \f$\left(0, 0, 1\right)\f$ to the one at \f$\left( 0, 0, -1 \right)\f$.
 */
class SpherePointSampler{

public:
    SpherePointSampler() = default;
    ~SpherePointSampler() = default;

    /**
     * \brief Sample \f$n\f$ points approximately uniformly on the surface of a unit sphere
     * 
     * Using the algorithm of Koay \cite Koay2011, this function samples \f$n\f$ points on the 
     * surface of the unit sphere, whose coordinates are identified by the polar angles
     * \f$\theta_i\f$
     * and the azimuthal angles \f$\varphi_i\f$, with \f$0 \leq i < n\f$.
     * 
     * \param n \f$n\f$, desired number of points
     * 
     * \return std::array that contains two std::vectors with size \f$n\f$. The first vector contains the values \f$\theta_i\f$, and the second vector contains the values \f$\varphi_i\f$.
     */
    array<vector<double>, 2> sample(const unsigned int n) const;

    /**
     * \brief Sample \f$n\f$ points approximately uniformly on the surface of a sphere in Cartesian coordinates
     * 
     * Compared to SpherePointSampler::sample, which returns pairs \f$\theta_i, \phi_i\f$,
     * this function returns the corresponding cartesian coordinates 
     * {see also the end of Sec. 2 in Ref. \cite Koay2011}:
     * 
     * \f[
     *      \mathbf{x}_i = \left(
     *          \begin{array}{c}
     *              r\mathrm{sin} \left( \theta_i \right) \mathrm{cos} \left( \varphi_i \right) \\
     *              r\mathrm{sin} \left( \theta_i \right) \mathrm{sin} \left( \varphi_i \right) \\
     *              r\mathrm{cos} \left( \theta_i \right)
     *          \end{array}
     *      \right).
     * \f]
     * 
     * Note that the radius \f$r\f$ of the sphere may be specified as well.
     * 
     * \param n \f$n\f$, desired number of points
     * \param r \f$r\f$, radius of the sphere
     * 
     * \return std::array that contains three std::vectors with size \f$n\f$, that contain the \f$x\f$-, \f$y\f$-, and \f$z\f$ coordinate. 
     */
    array<vector<double>, 3> sample_cartesian(const unsigned int n, const double r = 1.) const;

    /**
     * \brief Elliptic integral of the second kind \f$E\left( \varphi | m \right)\f$ for arbitrary real parameters
     * 
     * The length of the spiral trajectory, \f$S\left( \Theta \right)\f$, on which points are 
     * sampled in Ref. \cite Koay2011 [Eq. (4) therein]
     * can be expressed by incomplete elliptic integrals of the second kind \f$E\left( \varphi | m \right)\f$ {Eq. (5) in Ref. \cite Koay2011}:
     * 
     * \f[
     *      E\left( \varphi | m \right) = \int_0^{\varphi} \sqrt{ 1 - m \left[ \sin \left( \theta \right) \right]^2 } \mathrm{d} \theta.
     * \f]
     * 
     * where \f$\varphi\f$ is an angle, and \f$m\f$ is assumed to be a negative real number 
     * [\f$m = -c^2\f$, see Eq. (4)] in Ref. \cite Koay2011.
     * A definition of the elliptic integrals can be found in Sec. 17 of the 'Handbook of
     * Mathematical Functions' by Abramowitz and Stegun \cite AbramowitzStegun1974.
     * It indicates that \f$m\f$ may even be complex [see, e.g. the discussion below Eq. (17.2.18) therein].
     * The functions \f$E\left( \varphi | \theta \right)\f$ are implemented in the GNU Scientific 
     * Library (GSL) \cite Galassi2009 only for \f$ 0 \leq m < 1 \f$ with a parameter \f$k\f$,
     * 
     * \f[
     *      k^2 = m.
     * \f]
     * 
     * However, for any real parameter \f$m\f$ that is negative or has an absolute value larger 
     * than 1, the evaluation of the integral can be traced back to an evaluation in the interval 
     * \f$ 0 \leq m < 1 \f$ according to Eqs. (17.4.16) and (17.4.18) in 
     * \cite AbramowitzStegun1974.
     * These transformations assume a formulation of the incomplete elliptic integral in terms
     * of the parameter \f$u\f$ instead of \f$\varphi\f$, i.e. \f$E \left( u | m \right)\f$.
     * The quantity \f$u\f$ is the generalized angle \f$\varphi\f$ for the case of an ellipse
     * , such that, for example, an elliptic equivalent \f$\mathrm{sn}\f$ {a 'Jacobi Elliptic 
     * Function', see, e.g., Sec. 16 in Ref. \cite abromowitzStegun1974} of the trigonometric 
     * function \f$\sin\f$ can be defined {Eq. (17.2.2) in Ref. \cite AbramowitzStegun1974}:
     * 
     * \f[
     *      \mathrm{sn} \left( u \right) = \sin \left( \varphi \right).
     * \f]
     * 
     * The equation above provides a straightforward conversion from the Jacobi elliptic function 
     * \f$\mathrm{sn} \left( u \right)\f$ to \f$\varphi\f$ via the inverse sine.
     * The Jacobi elliptic functions are also implemented in GSL for the argument \f$u\f$,
     * but it should be noted that this implementation utilizes the parameter \f$m\f$ 
     * instead of \f$k\f$.
     * The conversion from \f$\varphi\f$ to \f$u\f$ is possible via the definition of the 
     * incomplete elliptic integral of the first kind {Eq. (17.2.7) in Ref. \cite AbramowitzStegun1974}:
     * 
     * \f[
     *      F \left( \varphi | m \right) = u.
     * \f]
     * 
     * For an arbitrary \f$m\f$, the function 
     * SpherePointSampler::elliptic_integral_2nd_kind_arbitrary_m will be called recursively 
     * with transformed parameters until the parameter can be handled by the GSL implementation.
     * 
     * In fact, the implementation does not only use the formalism of Abramowitz and Stegun, but 
     * the transformation to the interval \f$ 0 \leq m < 1 \f$ of the incomplete elliptic 
     * integral of the second kind is performed using 
     * Eq. (19.7.5) ('Imaginary Modulus Transformation') of the NIST Digital Library of 
     * Mathematical Functions (DLMF) \cite DLMF2020.
     * This equation has the advantage that it does not require the back-and-forth conversion 
     * between \f$\varphi\f$ and \f$u\f$, and that it handles \f$m < 0\f$ and \f$|m| > 1\f$ 
     * at the same time.
     * 
     * \param phi \f$\varphi\f$
     * \param m \f$m\f$
     * 
     * \return \f$E\left( \varphi | m \right)\f$
     */
    double elliptic_integral_2nd_kind_arbitrary_m(const double phi, const double m) const;

    /**
     * \brief Length of a spiral segment in the range \f$ 0 \leq \theta \leq \Theta \f$ by elliptic integral
     * 
     * Implements Eq. (4) in Ref. \cite Koay2011.
     * Note that the definitions of the elliptic integral in Refs. \cite AbramowitzStegun1974 and 
     * \cite Koay2011 are equivalent.
     * 
     * \param Theta \f$\Theta\f$
     * \param c \f$c\f$, proportionality constant between \f$\theta\f$ and \f$\varphi\f$
     * 
     * \return \f$S \left( \Theta, c \right)\f$
     */
    double segment_length(const double Theta, const double c) const;

   /**
     * \brief Length of a spiral segment in the range \f$ 0 \leq \theta \leq \Theta \f$ by linear interpolation
     * 
     * This function first samples \f$n\f$ points 
     * \f$\mathbf{x}_i\f$ with \f$0 \leq i \leq n - 1\f$ on the spiral trajectory, where the 
     * coordinates of the \f$i\f$-th point are given by
     * 
     * \f[
     *      \mathbf{x}_i = \left(
     *      \begin{array}{c}
     *          \sin \left( \theta_i \right) \cos \left( c\theta_i \right) \\
     *          \sin \left( \theta_i \right) \sin \left( c\theta_i \right) \\
     *          \cos \left( \theta_i \right)
     *      \end{array}
     * \right),
     * \f]
     * 
     * and the values \f$\theta_i\f$ are equidistant points between \f$\theta = 0\f$ and \f$\theta = \Theta\f$:
     * 
     * \f[
     *      \theta_i = \frac{i \Theta}{n - 1}.
     * \f]
     * 
     * By summing the geometric distances between neighboring points, an approximation of the 
     * segment length is obtained:
     * 
     * \f[
     *      S \left( \Theta \right) \approx \sum_{i = 0}^{n-2} \left| \mathbf{x}_{i+1} - \mathbf{x}_{i} \right|.
     * \f]
     * 
     * This function is used in the code for checking the correctness of the much 
     * more efficient method that uses elliptic integrals.
     * 
     * \param Theta \f$\Theta\f$
     * \param c \f$c\f$, proportionality constant between \f$\theta\f$ and \f$\varphi\f$
     * \param n_points \f$n\f$, number of points sampled on the spiral trajectory
     * 
     * \return \f$S \left( \Theta, c \right)\f$
     */
    double segment_length_linear_interpolation(const double Theta, const double c, const unsigned int n_points) const;

    /**
     * \brief For a desired number of points, find the optimum number of cycles of the spiral.
     * 
     * It is clear that the parameter \f$c\f$ depends on the number of desired points 
     * \f$n\f$, if the goal is to create the points as uniformly as possible.
     * Koay \cite Koay2011 suggests the relation [Eq. (9)]
     * 
     * \f[
     *      c = \frac{n \pi}{E\left( -c^2 \right)},
     * \f]
     * 
     * which follows from the demand that the length of each of the \f$n\f$ segments of the spiral,
     * \f$S \left( \pi \right) / n\f$, is equal to the distance on the spherical surface
     * between two points \f$\left( \theta, \varphi \right)\f$ and 
     * \f$\left( \theta^\prime, \varphi + 2 \pi \right)\f$, which is given by: 
     * 
     * \f[
     *      \theta^\prime - \theta = \frac{2 \pi}{c}.
     * \f]
     * 
     * The present implementation uses the iterative algorithm suggested in \cite Koay2011, 
     * Appendix A.
     * Convergence is assumed if the \f$j+1\f$-th element of the sequence differs from the 
     * \f$j\f$-th element by less than a number \f$epsilon\f$.
     * By default, the value \f$\epsilon = 10^{-8}\f$ from Ref. \cite Koay2011 is used.
     * 
     * \param n \f$n\f$, number of points to be sampled. Must be larger than 1.
     * \param epsilon Convergence criterion
     * \param max_n_iterations Maximum number of iterations. If the convergence criterion has not been reached after max_n_iterations iterations, an error is thrown.
     * 
     * \return \f$c\f$
     */
    double find_c(const unsigned int n, const double epsilon = 1e-8, const unsigned int max_n_iterations = 10000) const;

    /**
     * \brief Find the polar angle of the mid point of the j-th spiral segment for a given \f$c\f$
     * 
     * The mid points of the \f$j\f$-th of a total number of \f$n\f$ spiral segments can be found 
     * at segment lengths {Eq. (11) in \cite Koay2011}:
     * 
     * \f[
     *      S \left( \Theta_j \right) = \frac{(2j-1)\pi}{m},
     * \f]
     * 
     * where \f$j\f$ is a positive, nonzero integer in the range \f$ 1\leq j \leq n\f$.
     * 
     * The equation above is solved for \f$\Theta_j\f$ numerically, by using the iterative 
     * algorithm proposed by Koay \cite Koay2011.
     * See also the description of SpherePointSampler::find_c, which uses a similar iteration.
     * 
     * \param j \f$j\f$, index of the spiral segment whose mid point should be calculated
     * \param n \f$n\f$, number of points to be sampled which corresponds to the given \f$c\f$. Must be larger than 1.
     * \param c \f$c\f$, proportionality constant between \f$\theta\f$ and \f$\varphi\f$ which corresponds to the given \f$n\f$.
     * \param epsilon Convergence criterion
     * \param max_n_iterations Maximum number of iterations. If the convergence criterion has not been reached after max_n_iterations iterations, an error is thrown.
     * 
     * \return \f$\Theta_j\f$
     */
    double find_Theta_j(const unsigned int j, const unsigned int n, const double c, const double epsilon = 1e-8, const unsigned int max_n_iterations = 10000) const;

    /**
     * \brief Elliptic integral of the first kind \f$F\left( \varphi | m \right)\f$ for arbitrary real parameters
     * 
     * Needed for the determination of the optimum value for \f$c\f$ according to Eq. (14) 
     * in Ref. \cite Koay2011.
     * The transformation from arbitrary real \f$m\f$ to the range \f$0 \leq m < 1\f$ is given by
     * Eqs. (19.7.5) in Ref. \cite DLMF2020.
     * See also the definition of SpherePointSampler::elliptic_integral_2nd_kind_arbitrary_m
     * for more information.
     * 
     * \param phi \f$\varphi\f$
     * \param m \f$m\f$
     * 
     * \return \f$F\left( \varphi | m \right)\f$
     */
    double elliptic_integral_1st_kind_arbitrary_m(const double phi, const double m) const;
};