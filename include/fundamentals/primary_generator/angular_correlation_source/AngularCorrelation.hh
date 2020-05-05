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
#include <memory>
#include <utility>
#include <vector>

#include "EulerAngleRotation.hh"
#include "State.hh"
#include "Transition.hh"
#include "W_gamma_gamma.hh"

using std::array;
using std::pair;
using std::unique_ptr;
using std::vector;

/**
 * \brief Class for a gamma-gamma correlation.
 * 
 * Calculates the angular correlation \f$W_{\gamma \gamma} \left( \theta, \varphi \right)\f$ 
 * between the first and the last photon in a sequence of \f$n-1\f$ (\f$n > 2\f$) 
 * electromagnetic (EM) transitions between \f$n\f$ states of a quantized system 
 * (a 'cascade' of EM transitions with \f$n-1\f$ steps).
 * The angles \f$\theta \in \left[ 0, \pi \right]\f$ and 
 * \f$\varphi \in \left[ 0, 2 \pi \right]\f$ are the polar and azimuthal angles in a spherical
 * coordinate system, respectively.
 * States of the system are identified by their total angular momentum quantum numbers
 * \f$J\f$ (\f$2J \in \mathcal{N} \f$, \f$J \geq 0\f$) and their parities 
 * \f$\pi \in \lbrace -1, 1 \rbrace \f$.
 * They are labeled by indices \f$0 < i \geq n\f$, where \f$i = 1\f$ denotes the first, and
 * \f$i = n\f$ the last state of a cascade.
 * EM transitions are identified by their multipolarity \f$L\f$ (\f$L \in \mathcal{N} \f$, \f$L \geq 0\f$) and their EM character 
 * \f$\lambda \in \lbrace \mathrm{E}, \mathrm{M} \rbrace\f$ which can be either electric (E) or 
 * magnetic (M).
 * They are labeled by indices \f$1 \geq i < n\f$, where the \f$i\f$-th transition is assumed to be
 * the transition that connects the states labeled \f$i\f$ and \f$i+1\f$.
 * A transition between two states may be a mixture of up to two multipolarities \f$L_i\f$ and 
 * \f$L_i^\prime\f$, whose relative contributions are quantified by the corresponding 
 * multipole mixing ratio \f$\delta_i\f$ 
 * (see below about the convention for \f$\delta\f$ which is used in the present implementation).
 * 
 * The minimum possible cascade with \f$n = 3\f$ states is denoted symbolically as:
 * 
 * \f[
 *      J_1^{\pi_1} 
 *      \left( \begin{array}{ccc} L_1 \\ L_1^\prime \end{array} \right)
 *      J_2^{\pi_2}
 *      \left( \begin{array}{ccc} L_2 \\ L_2^\prime \end{array} \right)
 *      J_3^{\pi_3}.
 * \f]
 * 
 * A cascade between \f$n > 3\f$ states is denoted as:
 * 
 * \f[
 *      J_1^{\pi_1} 
 *      \left( \begin{array}{ccc} L_1 \\ L_1^\prime \end{array} \right)
 *      J_2^{\pi_2}
 *      \left( \begin{array}{ccc} L_2 \\ L_2^\prime \end{array} \right)
 *      ...
 *      J_{n-1}^{\pi_{n-1}}
 *      \left( \begin{array}{ccc} L_{n-1} \\ L_{n-1}^\prime \end{array} \right)
 *      J_n^{\pi_n}.
 * \f]
 * 
 * The transitions with \f$ 1 < i < n-1\f$ are assumed to be unobserved.
 * The photon which corresponds to the first (see below about the notion of 'first' and 'second') 
 * transition is assumed to be observed along the positive z direction (\f$\theta = 0\f$).
 * It may also be interpreted as a beam of photons which travels in that direction and causes 
 * the excitation of the state \f$J_2^{\pi_2}\f$ from the initial state \f$J_1^{\pi_1}\f$.
 * Apart from a correlation between the directions of motion of the two photons 
 * [direction-direction (dir-dir) correlation], the present implementation can take into 
 * account that the polarization of the first photon is observed in addition [a 
 * polarization-direction (pol-dir) correlation].
 * Here, it is assumed that the polarization is along the x axis 
 * (\f$\theta = \pi/2\f$, \f$\varphi = 0\f$ denotes the positive x axis).
 * Only the observation of polarization information, which introduces a dependence of the 
 * angular correlation on the azimuthal angle \f$\varphi\f$, allows for a distinction between 
 * different EM characters, which are also related to the parities of the corresponding states.
 * For this reason, the code will assume a pol-dir correlation if parities and EM 
 * characters associated with the first (the 'first' transition is well-defined in the user interface of the 
 * code) transition are given, and a dir-dir correlation otherwise.
 * 
 * The formalism of angular correlations which was used in the present implementation is mainly 
 * based on a review article by Fagg and Hanna 
 * \cite FaggHanna1959 and on a book chapter by Biedenharn \cite AjzenbergSelove1960.
 * Consequently, Biedenharn's convention for the multipole mixing ratio is used.
 * For a comparison to the other popular conventions of Rose and Brink and Krane, Steffen, and 
 * Wheeler, see Refs. \cite RoseBrink1967 and \cite KraneSteffenWheeler1973, respectively.
 * When a two-step cascade is considered in which the first and the last state are identical,
 * Biedenharn's convention has the advantage that \f$\delta_1 = \delta_2\f$.
 * 
 * In the angular correlation formalism, the expansion coefficients of \f$W_{\gamma \gamma}\f$
 * in terms of Legendre polynomials are separable into contributions by the different transitions.
 * Therefore, a 'first' and 'last' transition of the cascade actually need not be identified.
 * In other words, it does not matter whether the photon observed in z direction with an 
 * polarization in x direction is the first or the last one of the cascade.
 * However, the unobserved transitions must take place in between the two, otherwise they would 
 * not have to be considered anyway.
 * 
 * In order to describe an observation of the 'first' photon in an arbitrary direction with an 
 * arbitrary orientation of the polarization in the plane perpendicular to the direction of
 * propagation, the code allows to rotate the angular correlation by three Euler angles 
 * \f$\Phi\f$, \f$\Theta\f$, and \f$\Psi\f$.
 * They are defined to be the rotation angles around the z-, x', and z' axes, respectively, as 
 * described, for example, in Ref. \cite Weisstein2020.
 * 
 * Like many other quantum mechanical computer codes, this one also uses \f$2 J\f$ instead of 
 * \f$J\f$ and \f$2L\f$ instead of \f$L\f$ to be able to represent both integer and half-integer 
 * angular momentum quantum numbers internally as integers.
 * 
 * The angular correlation is normalized to \f$4\pi\f$, i.e.:
 * 
 * \f[
 *      \int_0^{2\pi} \int_0^\pi W_{\gamma \gamma} \left( \theta, \varphi \right) \sin \left( \theta \right) \mathrm{d} \theta \mathrm{d} \varphi = 4 \pi
 * \f]
 */
class AngularCorrelation{

public:
    /**
     * \brief Constructor
     * 
     * An EM cascade is specified entirely by the arguments of the constructor.
     * The first argument is the initial state of the cascade, which is sometimes denoted as the
     * 'oriented' state in the literature, because its decay/excitation defines the coordinate 
     * system.
     * All other cascade steps are given as a list of pairs of transitions and the states
     * which they populate.
     * The transition between the initial state and the second state, and the one between the
     * \f$n-1\f$-th and the \f$n\f$-th state, are assumed to be the two observed transitions.
     * All other transitions are treated as unobserved.
     * 
     * If the parities of the initial state and the second state, and both EM characters of the
     * first transition are given, the code will assume a pol-dir correlation.
     * If none of these data is given, the code will assume a dir-dir correlation.
     * 
     * The constructor checks the input data for consistency in terms of angular momentum coupling
     * and selection rules for EM transitions.
     * Further checks are performed by the constructors of the State and Transition classes.
     * 
     * \param ini_sta Initial state of the cascade.
     * \param cas_ste Cascade steps, given as a list of arbitrary length which contains 
     * Transition-State pairs. 
     * The first and the last transition of this list are assumed to be observed.
     * 
     * \throw invalid_argument if at least one of the following consistency checks fails: 
     * 1. The number of cascade steps is larger than one, because two transitions are needed 
     * for a gamma-gamma correlation.
     * 2. At least one multipolarity of a transition fulfils the triangle inequality with the 
     * angular momenta of the initial and final states of that transition.
     * 3. The EM characters of all multipolarities of a single transition fulfil
     * the parity selection rules.
     * In particular, if at least one parity or EM character is specified, all others must also
     * be specified for the given transition and the states which it links.
     * Note that this also applies to so-called 'pure' transitions where only a single 
     * multipolarity is allowed.
     */
    AngularCorrelation(const State ini_sta, const vector<pair<Transition, State>> cas_ste);
    ~AngularCorrelation() = default; /**< Destructor */

    /**
     * \brief Return the angular correlation for given spherical coordinates.
     * 
     * This function assumes that the direction of propagation of the first photon is in the
     * positive z direction.
     * If the correlation is a pol-dir correlation, the function assumes that the polarization axis
     * is the x axis.
     * 
     * \param theta Polar angle in spherical coordinates in radians 
     * (\f$\theta \in \left[ 0, \pi \right]\f$).
     * \param phi Azimuthal angle in spherical coordinates in radians 
     * (\f$\varphi \in \left[ 0, 2 \pi \right]\f$).
     * 
     * \return \f$W_{\gamma \gamma} \left( \theta, \varphi \right)\f$
     */
    double operator()(const double theta, const double phi) const;
    /**
     * \brief Return the angular correlation for an arbitrary coordinate system.
     * 
     * This function takes an array of three Euler angles as an addition parameter, to rotate 
     * the direction of propagation and the polarization axis (if defined) of the first photon.
     * The 'zxz' convention or 'x' convention is used for the order of the rotations 
     * \cite Weisstein2020.
     * If all Euler angles are set to zero, the direction of propagation is in the positive z 
     * direction, and the polarization axis (if defined) is the x axis.
     * As implied by the notation, the angles \f$\theta\f$ and \f$\varphi\f$ are still defined in
     * the original coordinate system, i.e. \f$\theta = 0\f$ is still the z axis.
     * 
     * \param theta Polar angle in spherical coordinates in radians 
     * (\f$\theta \in \left[ 0, \pi \right]\f$).
     * \param phi Azimuthal angle in spherical coordinates in radians 
     * (\f$\varphi \in \left[ 0, 2 \pi \right]\f$).
     * \param euler_angles Euler angles \f$\Phi\f$, \f$\Theta\f$, and \f$\Psi\f$ in radians which define an 
     * arbitrary rotation in 3D space in the x convention.
     * 
     * \return \f$W_{\gamma \gamma} \left( \theta, \varphi \right)\f$
     */
    double operator()(const double theta, const double phi, const array<double, 3> euler_angles) const;

protected:
/**
 * \brief Check consistency of the input.
 * 
 * Checks whether the cascade has more than one step.
 * After that, calls the functions AngularCorrelation::check_triangle_inequalities() and 
 * AngularCorrelation::check_em_transitions().
 * 
 * \param ini_sta Initial state of the cascade.
 * \param cas_ste Cascade steps, given as a list of arbitrary length which contains 
 * Transition-State pairs.
 * 
 * \throw invalid_argument if the cascade has less than two steps.
 */
    void check_cascade(const State ini_sta, const vector<pair<Transition, State>> cas_ste) const;

/**
 * \brief Check triangle inequality for all cascade steps.
 * 
 * Checks whether at least one of
 * 
 * \f[
 *      \left| J_i - J_{i+1} \right| \leq L_i \leq J_i + J_{i+1}
 * \f]
 * 
 * and 
 * 
 * \f[
 *      \left| J_i - J_{i+1} \right| \leq L_i^\prime \leq J_i + J_{i+1}
 * \f]
 * 
 * is fulfilled for each cascade step.
 * 
 * \param ini_sta Initial state of the cascade.
 * \param cas_ste Cascade steps, given as a list of arbitrary length which contains 
 * Transition-State pairs.
 * 
 * \throw invalid_argument if none of the two multipolarities fulfils the triangle inequality for any cascade step.
 */
    void check_triangle_inequalities(const State ini_sta, const vector<pair<Transition, State>> cas_ste) const;

/**
 * \brief Check parity selections rules for all cascade steps.
 * 
 * For each cascade step, first checks whether any of \f$\pi_i\f$, \f$\pi_{i+1}\f$, \f$\lambda_i\f$, 
 * \f$\lambda_i^\prime\f$ is given.
 * If yes, checks whether all of them are given.
 * If yes, checks whether the EM selections rules (see, e.g. Appendix 'Electromagnetic Transitions
 * and Moments' in Ref. \cite deShalitTalmi2004)
 * 
 * \f[
 *      \pi_i \pi_{i+1} = \left( -1 \right)^{L_i} ~~~ \to ~~~ \lambda_i = \mathrm{E}
 * \f]
 * 
 * and
 * 
 * \f[
 *      \pi_i \pi_{i+1} = \left( -1 \right)^{L_i+1} ~~~ \to ~~~ \lambda_i = \mathrm{M}
 * \f]
 * 
 * are fulfilled.
 * The same are also supposed to hold for \f$L_i^\prime\f$.
 * 
 * \param ini_sta Initial state of the cascade.
 * \param cas_ste Cascade steps, given as a list of arbitrary length which contains 
 * Transition-State pairs.
 * 
 * \throw invalid_argument if at least one of the following conditions is not fulfilled for any 
 * cascade step:
 * 1. Either all information about parities and EM characters for a cascade step is given, or none.
 * 2. The parity selection rules apply for both multipolarities of the transition.
 */
    void check_em_transitions(const State ini_sta, const vector<pair<Transition, State>> cas_ste) const;

/**
 * \brief Checks the parity selection rule for a single transition.
 * 
 * The transition of interest is assumed to have the label \f$i\f$.
 * 
 * \param p0 \f$p_i\f$Parity of initial state
 * \param p1 \f$p_{i+1}\f$Parity of final state
 * \param two_L \f$2L_i\f$, two times the multipolarity of the transition.
 * \param em \f$\lambda_i\f$ EM character of the transition with multipolarity \f$L\f$.
 * 
 * \return true, if the parity selection rule is fulfilled, false otherwise.
 */
    bool valid_em_character(const Parity p0, const Parity p1, const int two_L, const EMCharacter em) const;

    const EulerAngleRotation euler_angle_rotation; /**< Instance of the EulerAngleRotation class */
    /**
     * \brief Pointer to an object of the W_gamma_gamma class.
     * 
     * W_gamma_gamma is the base class of the W_dir_dir and W_pol_dir classes.
     * The user input decides which one is stored by this pointer.
     */
    unique_ptr<W_gamma_gamma> w_gamma_gamma;
};