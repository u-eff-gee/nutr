#pragma once

/**
 * \brief Class for an F coefficient.
 *
 * The definition used here is taken from Ref. \cite FerentzRosenzweig1955 which is referenced 
 * as 'unpublished' in Ref. \cite FaggHanna1959, but can usually be found quite easily \f$^1\f$.
 *
 * In \cite FerentzRosenzweig1955, the F coefficients are defined as [Eq. (1) therein]:
 *
 * \f[
 *	F_\nu \left( L, L^\prime, j_1, j \right) = 
 *	\left( -1 \right)^{j_1 - j - 1} 
 *	\sqrt{ \left( 2L + 1 \right) \left( 2L^\prime + 1 \right) \left( 2j + 1 \right) }
 *	\underbrace{
 *		\langle L, L^\prime, 1, -1 | \nu 0 \rangle
 *	}_{C^{L L^\prime \nu}_{1 \left( -1 \right) 0}}
 *	W \left( j, j, L, L^\prime; \nu j_1 \right).
 * \f]
 *
 * The expression includes a Clebsch-Gordan (CG) coefficient \f$C\f$ and a Racah coefficient 
 * \f$W\f$.
 * Both can be related to Wigner symbols, which are implemented in the GNU Scientific Library (GSL) 
 * \cite Galassi2009.
 * In particular, they are the Wigner-3j and 6j symbols, which will be displayed in matrix form with parentheses \f$\left( \right)\f$ and braces \f$\left\lbrace \right\rbrace\f$ here.
 * The relation between the CG coefficients and the Wigner-3j symbol is {Eq. (C.12) in \cite Messiah19622}:
 *
 * \f[
 *	\left(
 *		\begin{array}{ccc}
 *			j_1 & j_2 & J \\
 *			m_1 & m_2 & M
 *		\end{array}
 *	\right)
 *	= \frac{\left( -1 \right)^{j_1 - j_2 +M}}{\sqrt{2J + 1}} \langle j_1, j_2, m_1, m_2 | JM \rangle.
 * \f]
 *
 * The relation between the Racah coefficient and the Wigner-6j symbol is {Eq. (C.30) in \cite Messiah19622}:
 *
 * \f[
 *	\left\lbrace
 *		\begin{array}{ccc}
 *			j_1 & j_2 & j_3 \\
 *			J_1 & J_2 & J_3
 * 		\end{array}
 *	\right\rbrace
 *	= \left( -1 \right)^{j_1 + j_2 + J_1 + J_2} W(j_1, j_2, J_2, J_1; j_3 J_3).
 * \f]
 *
 * With these relations, the F coefficients can be expressed as:
 *
 * \f[
 *	F_\nu(L, L^\prime, j_1, j) = 
 *	\left( -1 \right)^{j_1 - j - 1} 
 *	\sqrt{ \left( 2L + 1 \right) \left( 2L^\prime + 1 \right) \left( 2j + 1 \right) }
 *	\underbrace{
 *		\sqrt{2 \nu + 1} \left( -1 \right)^{L - L^\prime}
 *		\left(
 *			\begin{array}{ccc}
 *				L & L^\prime & \nu \\
 *				1 & -1 & 0
 * 			\end{array}
 *		\right)
 *	}_{\langle L, L^\prime, 1, -1 | \nu 0 \rangle}
 *	\underbrace{
 *		\left( -1 \right)^{2j + L + L^\prime}
 *		\left\lbrace 
 *			\begin{array}{ccc}
 *				j & j & \nu \\
 *				L^\prime & L & j_1
 *			\end{array}
 *		\right\rbrace
 *	}_{W \left( j, j, L, L^\prime; \nu j_1 \right)}
 * \f]
 * \f[
 *	= \left( -1 \right)^{j_1 + j - 1} \sqrt{\left( 2L + 1 \right) \left( 2L^\prime + 1 \right) \left( 2j + 1 \right) \left( 2\nu + 1\right) }
 *		\left(
 *			\begin{array}{ccc}
 *				L & L^\prime & \nu \\
 *				1 & -1 & 0
 * 			\end{array}
 *		\right)
 *		\left\lbrace 
 *			\begin{array}{ccc}
 *				j & j & \nu \\
 *				L^\prime & L & j_1
 *			\end{array}
 *		\right\rbrace.
 * \f]
 *
 * For implementation details, see the documentation of FCoefficient::operator().
 *
 * \f$^1\f$ For better compatibility to the other literature used here, the label \f$k\f$ of
 * Ref. \cite FerentzRosenzweig1955 is replaced by \f$\nu\f$.
 */

class FCoefficient{
public:
	FCoefficient() = default;
	~FCoefficient() = default;

	/**
	 * \brief Return value of a specific F coefficient.
	 *
	 * The order of arguments is the same as in Eq. (1) in Ref. \cite FerentzRosenzweig1955.
	 * The parameter \f$\nu\f$ was assumed to be the first one.
	 * All parameters of this function correspond to their actual values multiplied by two, 
	 * to be able to work with integer numbers.
	 * This is in accordance with the definition of the Wigner symbols in GSL \cite Galassi2009.
	 *
	 * In the current implementation, the Wigner-3j and 6j symbols are calculated first, 
	 * and if any of them is zero, a zero is returned immediately to avoid further calculations.
	 *
	 * \param two_nu 2\f$\nu\f$
	 * \param two_L 2\f$L\f$
	 * \param two_Lp 2\f$L^\prime\f$
	 * \param two_j1 2\f$j_1\f$
	 * \param two_j 2\f$j\f$
	 *
	 * \return \f$F_\nu(L, L^\prime, j_1, j)\f$
	 */
	double operator()(const int two_nu, const int two_L, const int two_Lp, const int two_j1, const int two_j) const;
	
	/**
	 * \brief Check whether given F coefficient is nonzero.
	 *
	 * This function uses the analytical properties of the Clebsch-Gordan and Racah coefficients 
	 * to check efficiently whether a given set of parameters will lead to a nonzero F coefficient.
	 * Explicitly, it calls the two protected functions FCoefficient::cg_is_nonzero() and 
	 * FCoefficient::racah_is_nonzero().
	 *
	 * \param two_nu 2\f$\nu\f$
	 * \param two_L 2\f$L\f$
	 * \param two_Lp 2\f$L^\prime\f$
	 * \param two_j1 2\f$j_1\f$
	 * \param two_j 2\f$j\f$
	 *
	 * \return \f$F_\nu(L, L^\prime, j_1, j) != 0\f$
	 */
	bool is_nonzero(const int two_nu, const int two_L, const int two_Lp, const int two_j1, const int two_j);

protected:
	/**
	 * \brief Check whether given Clebsch-Gordan coefficient is nonzero.
	 *
	 * The criteria can be found in Appendix C.3 of Ref. \cite Messiah19622.
	 *
	 * \param two_j1 \f$2 j_1\f$
	 * \param two_j2 \f$2 j_2\f$
	 * \param two_J \f$2 J\f$
	 * \param two_m1 \f$2 m_1\f$
	 * \param two_m2 \f$2 m_2\f$
	 * \param two_M \f$2 M\f$
	 *
	 * \return \f[\left(
	 *		\begin{array}{ccc}
	 *			j_1 & j_2 & J \\
	 *			m_1 & m_2 & M
	 * 		\end{array}
	 *	\right) != 0\f]
	 */
	bool cg_is_nonzero(const int two_j1, const int two_j2, const int two_J, const int two_m1, const int two_m2, const int two_M) const;

	/**
	 * \brief Check whether given Racah coefficient is nonzero.
	 *
	 * The criteria can be found in Appendix C.7 of Ref. \cite Messiah19622.
	 *
	 * \param two_j1 \f$2 j_1\f$
	 * \param two_j2 \f$2 j_2\f$
	 * \param two_j3 \f$2 j_3\f$
	 * \param two_J1 \f$2 J_1\f$
	 * \param two_J2 \f$2 J_2\f$
	 * \param two_J3 \f$2 J_3\f$
	 *
	 * \return \f[\left\lbrace
	 *		\begin{array}{ccc}
	 *			j_1 & j_2 & j_3 \\
	 *			J_1 & J_2 & J_3
	 * 		\end{array}
	 *	\right\rbrace != 0\f]
	 */
	bool racah_is_nonzero(const int two_j1, const int two_j2, const int two_j3, const int two_J1, const int two_J2, const int two_J3) const;

	/**
	 * \brief Check whether three integers fulfil the triangle inequality.
	 *
	 * Given three integers \f$j_1\f$, \f$j_2\f$, and \f$J\f$, check whether 
	 *
	 * \f[
	 *	\left| j_1 - j_2 \right| <= J <= j_1 + j_2.
	 * \f]
	 *
	 * In the present implementation, the function does not check whether the given
	 * integers are positive, as commonly assumed when applying the triangle inequality.
	 *
	 * \param j1 \f$j_1\f$
	 * \param j2 \f$j_2\f$
	 * \param J \f$J\f$
	 *
	 * \return \f$ \left( \left| j_1 - j_2 \right| <= J \right) \&\& \left(J <= j_1 + j_2 \right)\f$
	 */
	bool fulfils_triangle_inequality(const int j1, const int j2, const int J) const;

	/**
	 * \brief Check whether the sum of three integers is an even number
	 *
	 * \param two_j1 \f$2 j_1\f$
	 * \param two_j2 \f$2 j_2\f$
	 * \param two_j3 \f$2 j_3\f$
	 * 
	 * \return \f$\left( 2 j_1 + 2 j_2 + 2 j_3 \right) \% 2 == 0\f$, 
	 * which is equivalent to '\f$\left( j_1 + j_2 + j_3 \right) \in \mathcal{Z} \f$' (where \f$\mathcal{Z}\f$ is the set of integer numbers)
	 */
	bool sum_is_even(const int two_j1, const int two_j2, const int two_j3) const { return ((two_j1 + two_j2 + two_j3) % 2 == 0); };
};
