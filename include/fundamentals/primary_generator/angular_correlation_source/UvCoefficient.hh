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
 * \brief Class for a \f$U_\nu\f$ coefficient
 * 
 * The \f$U_\nu\f$ coefficients are introduced, for example, in Eq. (I-1') of the review article 
 * by Fagg and Hanna \cite FaggHanna1959 to take into account unobserved emissions of 
 * radiation.
 * This means that the process in Eq. (I-1),
 * 
 * \f[
 *      j_1 \left( \begin{array}{c} L_1 \\ L_1^\prime \end{array} \right) j \left( \begin{array}{c} L_2 \\ L_2^\prime \end{array} \right) j_2,
 * \f]
 * 
 * which involves 3 nuclear states and 2 transitions, is extended to an \f$ n > 3\f$ - state process
 * with \f$ n - 1 \f$ transitions.
 * 
 * While the notation of Fagg and Hanna, which highlights the importance of the intermediate state 
 * \f$j\f$ by not assigning a label to it, is instructive for the case of 2 transitions, 
 * it may be confusing for cases with unobserved radiations.
 * Their definition of the coefficient \f$A_\nu(n)\f$ for the \f$n\f$-th transition [Eq. (I-2)] 
 * appears to always contain the angular momentum \f$j\f$, no matter what the value of \f$n\f$ is.
 * However, a comparison to other reviews, for example the explicit example given by Biedenharn in
 * Sec. 1.a.1.iii of their book chapter \cite AjzenbergSelove1960 (note that in this book, the 
 * \f$U_\nu\f$ coefficients are called \f$C_\nu\f$), shows that \f$j\f$ must be 
 * adapted as well.
 * For this reason, the nuclear states will be labeled in the order of their population in the 
 * cascade below to avoid misinterpretations.
 * First, relabel the equation above:
 * 
 * \f[
 *      j_1 \left( \begin{array}{c} L_1 \\ L_1^\prime \end{array} \right) 
 *      j_2 \left( \begin{array}{c} L_2 \\ L_2^\prime \end{array} \right)
 *      j_3.
 * \f]
 * 
 * The cascade with \f$n>3\f$ states is then denoted as:
 * 
 * \f[
 *      j_1 \left( \begin{array}{c} L_1 \\ L_1^\prime \end{array} \right) 
 *      j_2 \left( \begin{array}{c} L_2 \\ L_2^\prime \end{array} \right) 
 *      ~...~
 *      j_{n-1} \left( \begin{array}{c} L_{n-1} \\ L_{n-1}^\prime \end{array} \right)
 *      j_n
 * \f]
 * 
 * In this sequence, only the first transition from \f$j_1\f$ to \f$j_2\f$ and the last transition 
 * from \f$j_{n-1}\f$ to \f$j_n\f$ are assumed to be observed.
 * Each state with the label \f$1 < m <= n\f$ is populated by the transition with the label
 * \f$m - 1\f$.
 * 
 * The unobserved radiations are introduced into Eqs. (I-1) (direction-direction correlation)
 * and (I-8) (polarization-direction correlation) of Ref. \cite FaggHanna1959 by multiplying 
 * the so-called \f$U_\nu\f$ coefficients with the expansion coefficients for the (associated) 
 * Legendre polynomials of degree \f$\nu\f$.
 * In fact, it is not obvious from the article of Fagg and Hanna that this needs to be done for
 * terms proportional to the associated Legendre polynomials \f$P_\nu^{\left( 2 \right)}\f$ in 
 * Eq. (I-8) as well, because the 
 * \f$U_\nu\f$ coefficients are mentioned long before Eq. (I-8).
 * There is also no statement like that in Sec. 1.a.2 of Ref. \cite AjzenbergSelove1960, which 
 * treats polarization-direction correlations
 * (Note that Sec. 1.b in \cite AjzenbergSelove1960 is called 'Several Intermediate Nuclear 
 * States', but it treats a different concept than the 'unobserved radiations' discussed here).
 * A clear statement can be found, for example, in a review article by Krane, Steffen and Wheeler 
 * \cite KraneSteffenWheeler1973.
 * In this article, the expansion coefficients are generalized to so-called
 * \f$A_\lambda^{\lambda_2 \lambda_1}\f$ coefficients [Eq. (43)], which reduce to the \f$U_\nu\f$ 
 * coefficients or the \f$A_\nu\f$ coefficients (note that they have different names and probably
 * different phase conventions there) depending on whether a radiation is observed or not.
 * Although Krane, Steffen and Wheeler do not treat polarization, 'unobserved' implies that neither
 * the direction nor the polarization of that radiation are observed, therefore their replacement 
 * rules are sufficient for the present implementation.
 * 
 * The \f$U_\nu \left( m \right)\f$ coefficient associated with the unobserved transition 
 * 
 * \f[
 *      j_m \left( \begin{array}{c} L_{m+1} \\ L_{m+1}^\prime \end{array} \right) j_{m+1}, ~~~~ 1 < m < n-1
 * \f]
 * 
 * is then given by {Eq. (I-1') and the expression below that one in Ref. \cite FaggHanna1959}:
 * 
 * \f[
 *      U_\nu \left( m \right) = 
 *      \left( -1 \right)^{j_{m+1} - j_m - L_m} 
 *      \sqrt { \left( 2 j_{m+1} + 1 \right) \left( 2 j_m + 1 \right) }
 *      W \left( j_m, j_m, j_{m+1}, j_{m+1}; \nu, L_m \right)
 * \f]
 * \f[
 *      + \delta_m^2 \left( -1 \right)^{j_{m+1} - j_m - L_m^\prime} 
 *      \sqrt { \left( 2 j_{m+1} + 1 \right) \left( 2 j_m + 1 \right) }
 *      W \left( j_m, j_m, j_{m+1}, j_{m+1}; \nu, L_m^\prime \right)
 * \f]
 * 
 * Here, \f$\delta_m\f$ denotes the mixing ratio of the two multipolarities that may
 * contribute to the \f$m\f$-th transition.
 * The Racah coefficients \f$W\f$ are related to the Wigner-6j symbols via 
 * {see, e.g., Eq. (C.30) in \cite Messiah19622}:
 * 
 * \f[
 *      W \left(j_m, j_m, j_{m+1}, j_{m+1}; \nu, L_m \right) = \left( -1 \right)^{2 j_m + 2 j_{m+1}} \Big \lbrace \begin{array}{ccc} j_m & j_m & \nu \\ j_{m+1} & j_{m+1} & L_m \end{array} \Big \rbrace
 * \f]
 * \f[
 *      = \Big \lbrace \begin{array}{ccc} j_m & j_m & \nu \\ j_{m+1} & j_{m+1} & L_m \end{array} \Big \rbrace
 * \f]
 * 
 * The phase factor vanishes, because \f$j_m\f$ or \f$j_{m+1}\f$ are either both half-integer 
 * numbers or both integer numbers.
 * In both cases, \f$2 j_m + 2 j_{m+1} \f$ will be an even integer.
 * 
 * According to the properties of the Wigner-6j symbol {see, e.g., Sec. C.II.7 in 
 * \cite Messiah19622}, the coefficients \f$U_\nu\f$ are nonzero for:
 * 
 * \f[
 *      0 \leq \nu \leq 2 j_m
 * \f]
 * \f[
 *      0 \leq \nu \leq 2 j_{m+1},
 * \f] 
 * 
 * i.e. they may terminate the sum over \f$\nu\f$ in the calculation of the angular correlation.
 * In particular, the sum is reduced to the zero-order term (i.e. isotropic emission) if states 
 * with an angular momentum quantum number of 0 or 1/2 are part of the cascade.
 * 
 * It may also be instructive to show the \f$A_\nu\f$ coefficients explicitly which appear in the 
 * case \f$n > 3\f$ {see, e.g., Eqs. (67a) and (67b) in Ref. \cite AjzenbergSelove1960. Note that 
 * their labels start from \f$m = 0\f$ in contrast to the notation here}:
 * 
 * \f[
 *      A_\nu \left( 1 \right) = 
 *          F_\nu \left( L_1, L_1, j_1, j_2 \right)
 *          + 2 \delta_1 F_\nu \left( L_1, L_1^\prime, j_1, j_2 \right)
 *          + \delta_1^2 F_\nu \left( L_1^\prime, L_1^\prime, j_1, j_2 \right)
 * \f]
 * \f[
 *      A_\nu \left( n-1 \right) = 
 *          F_\nu \left( L_{n-1}, L_{n-1}, j_n, j_{n-1} \right)
 *          + 2 \delta_{n-1} F_\nu \left( L_{n-1}, L_{n-1}^\prime, j_n, j_{n-1} \right)
 *          + \delta_{n-1}^2 F_\nu \left( L_{n-1}^\prime, L_{n-1}^\prime, j_n, j_{n-1} \right).
 * \f]
 */
class UvCoefficient{
public:
    UvCoefficient() = default;
    ~UvCoefficient() = default;

    /**
     * \brief Return value of a specific \f$U_\nu\f$ coefficient.
     * 
     * The order of arguments is meant to resemble the notation
     * 
     * \f[
     *      j_m \left( \begin{array}{c} L_{m+1} \\ L_{m+1}^\prime \end{array} \right) j_{m+1}.
     * \f]
     * 
     * \param two_nu \f$2 \nu\f$
     * \param two_j \f$2 j_m\f$
     * \param two_L \f$2 L_m\f$
     * \param two_jp \f$2 j_{m+1}\f$
     * 
     * \return \f$U_\nu \left( m \right)\f$
     */
    double operator()(const unsigned int two_nu, const int two_j, const int two_L, const int two_jp) const;
};