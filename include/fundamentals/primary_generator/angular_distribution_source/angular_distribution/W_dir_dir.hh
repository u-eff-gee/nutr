#pragma once

#include <vector>

#include "AvCoefficient.hh"
#include "State.hh"
#include "Transition.hh"

using std::vector;

/**
 * \brief Class for a direction-direction (dir-dir) correlation
 *
 * Evaluates the angular correlation between two photons in a nuclear reactions of the type 
 * \f$(\gamma, \gamma')\f$ in which a nucleus in an initial state with the spin 
 * \f$j_1\f$ absorbs a photon to render it in an excited state with the spin \f$j\f$, 
 * and emits another photon in a transition to another state with the spin \f$j_2\f$.
 * It is assumed that only the direction of the two photons in the process is known,
 * but no polarization information.
 * In particular, this means that the dir-dir correlation does not depend on the parity
 * quantum number of the involved nuclear states.
 * 
 * The class uses Eqs. (I-1) and (I-2) of Ref. \cite FaggHanna1959 with a similar notation.
 * Note that Eq. (I-2) already includes the assumption that only two multipoles contribute
 * to any of the two transitions, and that the spin of the intermediate state is known.
 *
 */
class W_dir_dir{
public:
	W_dir_dir(const State ini_state, const Transition ini_to_int, const State int_state, const Transition int_to_fin, const State fin_state);
	~W_dir_dir() = default;

	double operator()(const double theta);

protected:
	/**
	 * \brief Get the maximum value \f$\nu_\mathrm{max}\f$ for which the coefficient 
	 * \f$A_\nu\f$ is nonzero
	 *
	 * The maximum value \f$\nu_\mathrm{max}\f$ for which the coefficients \f$A_\nu\f$ does not 
	 * vanish is obtained here by considering the selection rules of the F coefficients, and
	 * more specifically the selection rules of the underlying Clebsch-Gordan and Racah
	 * coefficients.
	 *
	 * At first, note that the assumption of two multipole orders and a known spin of the
	 * intermediate state lead to the restriction {mentioned above Eq. (I-2) in 
	 * Ref. \cite FaggHanna1959}:
	 *
	 * \f[
	 *	\nu \% 2 = 0,
	 * \f]
	 *
	 * i.e. \f$\nu\f$ can only be an even integer number.
	 *
	 * The following Clebsch-Gordan coefficients appear in the definition of a single 
	 * \f$A_\nu\f$ coefficient {see Eq. (I-2) in \cite FaggHanna1959 and Eq. (1) in 
	 * \cite FerentzRosenzweig1955}:
	 *
	 * \f[
	 * 	\left(
	 *		\begin{array}{ccc}
	 *			L_n & L_n & \nu \\
	 *			1 & -1 & 0
	 *		\end{array}
	 *	\right)
	 * \f]
	 * \f[
	 * 	\left(
	 *		\begin{array}{ccc}
	 *			L_n & L_n^\prime & \nu \\
	 *			1 & -1 & 0
	 *		\end{array}
	 *	\right)
	 * \f]
	 * \f[
	 * 	\left(
	 *		\begin{array}{ccc}
	 *			L_n^\prime & L_n^\prime & \nu \\
	 *			1 & -1 & 0
	 *		\end{array}
	 *	\right)
	 * \f]
	 *
	 * The selection rules for the Clebsch-Gordan coefficients above that involve \f$\nu\f$ are
	 * {Sec. C.I.3 in \cite Messiah19622}:
	 *
	 * \f[
	 *	\left| L_n - L_n \right| = 0 \leq \nu \leq 2 L_n
	 * \f]
	 * \f[
	 *	\left| L_n - L_n^\prime \right| \leq \nu \leq L_n + L_n^\prime
	 * \f]
	 * \f[
	 *	\left| L_n^\prime - L_n^\prime \right| = 0 \leq \nu \leq 2 L_n^\prime.
	 * \f]
	 *
	 * In principle, there is also the requirement that \f$ M \leq J\f$ 
	 * (in the notation of Ref. \cite Messiah19622), which is always fulfilled here since
	 * \f$M = 0\f$.
	 * From the selection rules, it can be concluded that:
	 *
	 * \f[
	 *	0 \leq \nu \leq \max \left( 2 L_n, 2 L_n^\prime \right).
	 * \f]
	 *
	 * In addition, the Wigner-6j symbols (Racah coefficients without the phase factor)
	 *
	 * \f[
	 *	\left\lbrace
	 *		\begin{array}{ccc}
	 *			j & j & \nu \\
	 *			L_n & L_n & j_n
	 *		\end{array}
	 *	\right\rbrace
	 * \f]
	 * \f[
	 *	\left\lbrace
	 *		\begin{array}{ccc}
	 *			j & j & \nu \\
	 *			L_n^\prime & L_n & j_n
	 *		\end{array}
	 *	\right\rbrace
	 * \f]
	 * \f[
	 *	\left\lbrace
	 *		\begin{array}{ccc}
	 *			j & j & \nu \\
	 *			L_n^\prime & L_n^\prime & j_n
	 *		\end{array}
	 *	\right\rbrace,
	 * \f]
	 *
	 * which appear in a coefficient \f$A_\nu\f$ {see Eq. (I-2) in \cite FaggHanna1959 and 
	 * Eq. (1) in \cite FerentzRosenzweig1955} provide the following selection rules 
	 * for \f$\nu\f$ {Sec. C.II.7 in \cite Messiah19622}:
	 * 
	 * \f[
	 *	\left| j - j \right| = 0 \leq \nu \leq 2j
	 * \f]
	 * \f[
	 *	2j + \nu ~~~~ \mathrm{integer}
	 * \f]
	 * \f[
	 *	2L_n + \nu ~~~~ \mathrm{integer}
	 * \f]
	 * \f[
	 *	L_n + L_n^\prime + \nu ~~~~ \mathrm{integer}
	 * \f]
	 * \f[
	 *	2L_n^\prime + \nu ~~~~ \mathrm{integer}.
	 * \f]
	 *
	 * They also impose the same triangle inequalities as the Clebsch-Gordan 
	 * coefficients via the relation between \f$J_1\f$, \f$J_2\f$, 
	 * and \f$J_3\f$ (in the notation of Ref. \cite Messiah19622).
	 * All conditions of the type '\f$... + \nu\f$ is integer' confirm only that 
	 * \f$\nu\f$ is an integer, since the multipolarities \f$L_n\f$ and \f$L_n^\prime\f$, 
	 * as well as the quantity \f$2j\f$ are integers.
	 *
	 * In summary, the coefficients \f$A_\nu\f$ will be nonzero if
	 *
	 * \f[
	 *	\nu \% 2 = 0,
	 * \f]
	 * \f[
	 *	0 \leq \nu \leq \min \left[ 2 j, \max \left( 2 L_n, 2 L_n^\prime \right) \right],
	 * \f]
	 *
	 * since at least of the three F coefficients will have a nonzero value.
	 * The expression above is a algebraically correct.
	 * Nevertheless, physical applications neglect the terms where, without loss of 
	 * generality:
	 *
	 * \f[
	 *	L_n < \nu_i \leq L_n^\prime, 2j.
	 * \f]
	 *
	 * In this case, only the term proportional to \f$\delta_n^2\f$ in \f$A_{\nu_i}\f$ 
	 * {Eq. (I-2) in Ref. \cite FaggHanna1959} remains, which describes the contribution 
	 * by the higher-order multipole.
	 * The author assumes that these contributions are not included because they belong
	 * to a higher-order mixing of multipoles which goes beyond the assumed mixing of only
	 * two multipolarities.
	 * Therefore, \f$\nu\f$ is usually restricted to:
	 *
	 * \f[
	 *	0 \leq \nu \leq \min \left( 2 j, 2 L_n, 2 L_n^\prime \right).
	 * \f]
	 *
	 * Considering that every term in the summation over \f$\nu\f$ in the expression for
	 * \f$W\left( \theta \right)\f$ includes a product of \f$A_\nu\f$ coefficients for
	 * \f$n=1\f$ and \f$n=2\f$ {Eq. (I-2) in Ref. \cite FaggHanna1959}, the value of 
	 * \f$\nu\f$ is restricted to:
	 *
	 * \f[
	 *	0 \leq \nu \leq \min \left[ 2 j, \min\left( L_1, L_1^\prime \right), \min\left( L_2, L_2^\prime \right) \right].
	 * \f]
	 *
	 * The equation above is more general than Eq. (13) in Ref. \cite FerentzRosenzweig1955,
	 * since it does not make an assumption about the relative magnitude of \f$L_n\f$ and
	 * \f$L_n^\prime\f$.
	 *
	 */
	int get_two_nu_max(const int two_L_n, const int two_L_p) const;
	vector<double> calculate_av_coefficients();

	const AvCoefficient av_coef;
	vector<double> av_coef_cache;

	Transition initial_to_intermediate;
	Transition intermediate_to_final;
	State initial_state;
	State intermediate_state;
	State final_state;

	int two_nu_max; 
};
