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
 * \brief Class for a polarization coefficient
 * 
 * The coefficients \f$\kappa_\nu\f$ are introduced in Eq. (I-4) of Ref. \cite FaggHanna1959
 * to take into account polarization information, which is probably why they are called 
 * polarization coefficients in \cite Kneissl1996.
 * They are defined as {Eq. (I-7) in \cite FaggHanna1959}:
 * 
 * \f[
 *      \kappa_\nu \left( L, L^\prime \right) = - \sqrt{ \frac{ \left( \nu - 2 \right)! }{ \left( \nu + 2 \right)! } } 
 * \frac{ 
 *      \left(
 *		    \begin{array}{ccc}
 *			    L & L^\prime & \nu \\
 *			    1 & 1 & 2
 *		    \end{array}
 *	    \right)
 * }{
 *      \left(
 *		    \begin{array}{ccc}
 *			    L & L^\prime & \nu \\
 *			    1 & -1 & 0
 *		    \end{array}
 *	    \right).
 * }
 * \f]
 * 
 * The definition includes two Clebsch Gordan coefficients.
 * From the definition, it is obvious that one will run into problems for $\nu = 0$, 
 * since two different angular momenta \f$L\f$ and \f$L^\prime\f$ can not be coupled to zero,
 * and the behavior of the factorial for negative numbers is undefined (The latter problem persists
 * for \f$\nu = 1\f$. However, this case is not encountered here).
 * Reference \cite FaggHanna1959 and Ref. \cite LLoyd1952, which is cited by the former for 
 * \f$\kappa_\nu\f$, only give values for the coefficients with \f$\nu > 2\f$ 
 * [see Tab. II(b) \cite FaggHanna1959 and Tab. III \cite LLoyd1952, respectively, and note 
 * the different normalization].
 * Equation (I-8) of \cite FaggHanna1959 mentions a restriction to
 * even, nonzero values of \f$\nu\f$ when the dir-dir correlation is extended to include 
 * polarization, but does not attribute this to the \f$\kappa_\nu\f$ coefficients.
 * 
 * In this implementation, it was decided to throw an exception when \f$\kappa_{\nu < 2}\f$
 * is requested.
 */
class KappaCoefficient{
public:
	/**
	 * \brief Constructor
	 */
	KappaCoefficient() = default;
	/**
	 * \brief Destructor
	 */
	~KappaCoefficient() = default;

    /**
     * \brief Return value of a specific polarization coefficent
     * 
     * \param two_nu \f$2 \nu\f$
     * \param two_L \f$2 L\f$
     * \param two_Lp \f$2 L^\prime \f$
     * 
     * \return \f$\kappa_\nu \left( L, L^\prime \right) \f$
     * 
     * \exception std::invalid_argument If \f$\nu < 2\f$, i.e. \f$2\nu < 4\f$
     */
	double operator()(const int two_nu, const int two_L, const double two_Lp) const;

};
