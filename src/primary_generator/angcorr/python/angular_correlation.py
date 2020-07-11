# This file is part of nutr.

# nutr is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# nutr is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with nutr.  If not, see <https://www.gnu.org/licenses/>.

# Copyright (C) 2020 Udo Friman-Gayer

## @package angular_correlation
# Module for the calculation of angular correlations between two photons
#
# Wraps C++ code of this package by using the python ctypes module.
# Since ctypes imports C, and not C++, code, the functor classes of the C++ code cannot be used
# in a straightforward way, but each call of angular_distribution creates a new instance of the 
# corresponding AngularCorrelation class.
# This leads to repetitive calculations of the expansion coefficients of the angular correlations
# and definitely impacts the performance.

from ctypes import cdll, c_double, c_int, c_short, c_size_t, POINTER

from state import *
from transition import *

lib = cdll.LoadLibrary('@PROJECT_BINARY_DIR@/src/primary_generator/angcorr/libangular_correlation.so')

lib.angular_correlation.restype = c_double
lib.angular_correlation.argtypes = [
    c_double,          # Polar angle theta
    c_double,          # Azimuthal angle phi
    c_size_t,          # Number of cascade steps
    POINTER(c_int),    # Angular momenta
    POINTER(c_short),  # Parities
    POINTER(c_short),    # EM characters
    POINTER(c_int),    # Multipolarities
    POINTER(c_short),    # Alternative EM characters
    POINTER(c_int),    # Alternative multipolarities
    POINTER(c_double), # Multipole mixing ratios
]


lib.angular_correlation_rotated.restype = c_double
lib.angular_correlation_rotated.argtypes = [
    c_double,          # Polar angle theta
    c_double,          # Azimuthal angle phi
    c_size_t,          # Number of cascade steps
    POINTER(c_int),    # Angular momenta
    POINTER(c_short),  # Parities
    POINTER(c_short),  # EM characters
    POINTER(c_int),    # Multipolarities
    POINTER(c_short),  # Alternative EM characters
    POINTER(c_int),    # Alternative multipolarities
    POINTER(c_double), # Multipole mixing ratios
    c_double,          # Euler angle Phi
    c_double,          # Euler angle Theta
    c_double,          # Euler angle Psi
]

def angular_correlation(theta, phi, 
    initial_state = State(0, POSITIVE), 
    cascade_steps = [
        [Transition(ELECTRIC, 2, MAGNETIC, 4, 0.), State(2, NEGATIVE)],
        [Transition(EM_UNKNOWN, 2, EM_UNKNOWN, 4, 0.), State(0, PARITY_UNKNOWN)],
    ],
    Phi = 0., Theta = 0., Psi = 0.
):
    """Class for a gamma-gamma correlation.

    Calculates the angular correlation \f$W_{\gamma \gamma} \left( \theta, \varphi \right)\f$ 
    between the first and the last photon in a sequence of \f$n-1\f$ (\f$n > 2\f$) 
    electromagnetic (EM) transitions between \f$n\f$ states of a quantized system 
    (a 'cascade' of EM transitions with \f$n-1\f$ steps).
    The angles \f$\theta \in \left[ 0, \pi \right]\f$ and 
    \f$\varphi \in \left[ 0, 2 \pi \right]\f$ are the polar and azimuthal angles in a spherical
    coordinate system, respectively.
    States of the system are identified by their total angular momentum quantum numbers
    \f$J\f$ (\f$2J \in \mathcal{N} \f$, \f$J \geq 0\f$) and their parities 
    \f$\pi \in \lbrace -1, 1 \rbrace \f$.
    They are labeled by indices \f$0 < i \geq n\f$, where \f$i = 1\f$ denotes the first, and
    \f$i = n\f$ the last state of a cascade.
    EM transitions are identified by their multipolarity \f$L\f$ (\f$L \in \mathcal{N} \f$, \f$L \geq 0\f$) and their EM character 
    \f$\lambda \in \lbrace \mathrm{E}, \mathrm{M} \rbrace\f$ which can be either electric (E) or 
    magnetic (M).
    They are labeled by indices \f$1 \geq i < n\f$, where the \f$i\f$-th transition is assumed to be
    the transition that connects the states labeled \f$i\f$ and \f$i+1\f$.
    A transition between two states may be a mixture of up to two multipolarities \f$L_i\f$ and 
    \f$L_i^\prime\f$, whose relative contributions are quantified by the corresponding 
    multipole mixing ratio \f$\delta_i\f$ 
    (see below about the convention for \f$\delta\f$ which is used in the present implementation).

    The minimum possible cascade with \f$n = 3\f$ states is denoted symbolically as:

    \f[
        J_1^{\pi_1} 
        \left( \begin{array}{ccc} L_1 \\ L_1^\prime \end{array} \right)
        J_2^{\pi_2}
        \left( \begin{array}{ccc} L_2 \\ L_2^\prime \end{array} \right)
        J_3^{\pi_3}.
    \f]

    A cascade between \f$n > 3\f$ states is denoted as:

    \f[
        J_1^{\pi_1} 
        \left( \begin{array}{ccc} L_1 \\ L_1^\prime \end{array} \right)
        J_2^{\pi_2}
        \left( \begin{array}{ccc} L_2 \\ L_2^\prime \end{array} \right)
        ...
        J_{n-1}^{\pi_{n-1}}
        \left( \begin{array}{ccc} L_{n-1} \\ L_{n-1}^\prime \end{array} \right)
        J_n^{\pi_n}.
    \f]

    The transitions with \f$ 1 < i < n-1\f$ are assumed to be unobserved.
    The photon which corresponds to the first (see below about the notion of 'first' and 'second') 
    transition is assumed to be observed along the positive z direction (\f$\theta = 0\f$).
    It may also be interpreted as a beam of photons which travels in that direction and causes 
    the excitation of the state \f$J_2^{\pi_2}\f$ from the initial state \f$J_1^{\pi_1}\f$.
    Apart from a correlation between the directions of motion of the two photons 
    [direction-direction (dir-dir) correlation], the present implementation can take into 
    account that the polarization of the first photon is observed in addition [a 
    polarization-direction (pol-dir) correlation].
    Here, it is assumed that the polarization is along the x axis 
    (\f$\theta = \pi/2\f$, \f$\varphi = 0\f$ denotes the positive x axis).
    Only the observation of polarization information, which introduces a dependence of the 
    angular correlation on the azimuthal angle \f$\varphi\f$, allows for a distinction between 
    different EM characters, which are also related to the parities of the corresponding states.
    For this reason, the code will assume a pol-dir correlation if parities and EM 
    characters associated with the first (the 'first' transition is well-defined in the user interface of the 
    code) transition are given, and a dir-dir correlation otherwise.

    The formalism of angular correlations which was used in the present implementation is mainly 
    based on a review article by Fagg and Hanna 
    \cite FaggHanna1959 and on a book chapter by Biedenharn \cite AjzenbergSelove1960.
    Consequently, Biedenharn's convention for the multipole mixing ratio is used.
    For a comparison to the other popular conventions of Rose and Brink and Krane, Steffen, and 
    Wheeler, see Refs. \cite RoseBrink1967 and \cite KraneSteffenWheeler1973, respectively.
    When a two-step cascade is considered in which the first and the last state are identical,
    Biedenharn's convention has the advantage that \f$\delta_1 = \delta_2\f$.

    In the angular correlation formalism, the expansion coefficients of \f$W_{\gamma \gamma}\f$
    in terms of Legendre polynomials are separable into contributions by the different transitions.
    Therefore, a 'first' and 'last' transition of the cascade actually need not be identified.
    In other words, it does not matter whether the photon observed in z direction with an 
    polarization in x direction is the first or the last one of the cascade.
    However, the unobserved transitions must take place in between the two, otherwise they would 
    not have to be considered anyway.

    In order to describe an observation of the 'first' photon in an arbitrary direction with an 
    arbitrary orientation of the polarization in the plane perpendicular to the direction of
    propagation, the code allows to rotate the angular correlation by three Euler angles 
    \f$\Phi\f$, \f$\Theta\f$, and \f$\Psi\f$.
    They are defined to be the rotation angles around the z-, x', and z' axes, respectively, as 
    described, for example, in Ref. \cite Weisstein2020.

    Like many other quantum mechanical computer codes, this one also uses \f$2 J\f$ instead of 
    \f$J\f$ and \f$2L\f$ instead of \f$L\f$ to be able to represent both integer and half-integer 
    angular momentum quantum numbers internally as integers.

    The angular correlation is normalized to \f$4\pi\f$, i.e.:

    \f[
        \int_0^{2\pi} \int_0^\pi W_{\gamma \gamma} \left( \theta, \varphi \right) \sin \left( \theta \right) \mathrm{d} \theta \mathrm{d} \varphi = 4 \pi
    \f]

    An EM cascade is specified entirely by the arguments of angular_correlation which follow the
    angles in spherical coordinates.
    The third argument is the initial state of the cascade, which is sometimes denoted as the
    'oriented' state in the literature, because its decay/excitation defines the coordinate 
    system.
    All other cascade steps are given as a list of pairs of transitions and the states
    which they populate.
    The transition between the initial state and the second state, and the one between the
    \f$n-1\f$-th and the \f$n\f$-th state, are assumed to be the two observed transitions.
    All other transitions are treated as unobserved.
    
    If the parities of the initial state and the second state, and both EM characters of the
    first transition are given, the code will assume a pol-dir correlation.
    If none of these data is given, the code will assume a dir-dir correlation.
    
    The angular_correlation function checks the input data for consistency in terms of angular 
    momentum coupling and selection rules for EM transitions.
    Further checks are performed by the constructors of the State and Transition classes.

    If no parameters for a rotation are given (see below), this function assumes that the 
    direction of propagation of the first photon is in the positive z direction.
    If the correlation is a pol-dir correlation, the function assumes that the polarization axis
    is the x axis.

    As mentioned above, this function can take three Euler angles as additional parameters, to 
    rotate the direction of propagation and the polarization axis (if defined) of the first photon.
    The 'zxz' convention or 'x' convention is used for the order of the rotations 
    \cite Weisstein2020.
    As implied by the notation, the angles \f$\theta\f$ and \f$\varphi\f$ are still defined in
    the original coordinate system, i.e. \f$\theta = 0\f$ is still the z axis.

    Parameters
    ----------
    theta: float 
        Polar angle in spherical coordinates in radians (\f$\theta \in \left[ 0, \pi \right]\f$).
    phi: float 
        Azimuthal angle in spherical coordinates in radians (\f$\varphi \in \left[ 0, 2 \pi \right]\f$).
    initial_state: State
        Initial state of the cascade.
    cas_ste: array of [Transition, State] pairs
        Cascade steps, given as a list of arbitrary length which contains Transition-State pairs. 
        The first and the last transition of this list are assumed to be observed.
    Phi: float
        Euler angle \f$\Phi\f$ in radians (default: 0.).
    Theta: float
        Euler angle \f$\Theta\f$ in radians (default: 0.).   
    Psi: float
        Euler angle \f$\Psi\f$ in radians (default: 0.).

    Returns
    -------
    float
        \f$W_{\gamma \gamma} \left( \theta, \varphi \right)\f$
    """

    n_cas_ste = len(cascade_steps)
    two_J = [cas_ste[1].two_J for cas_ste in cascade_steps]
    two_J.insert(0, initial_state.two_J)
    two_J = (c_int * len(two_J))(*two_J)
    par = [cas_ste[1].parity for cas_ste in cascade_steps]
    par.insert(0, initial_state.parity)
    par = (c_short * len(par))(*par)

    em_char = [cas_ste[0].em_char for cas_ste in cascade_steps]
    em_char = (c_short * len(em_char))(*em_char)
    two_L = [cas_ste[0].two_L for cas_ste in cascade_steps]
    two_L = (c_int * len(two_L))(*two_L)
    em_charp = [cas_ste[0].em_charp for cas_ste in cascade_steps]
    em_charp = (c_short * len(em_charp))(*em_charp)
    two_Lp = [cas_ste[0].two_Lp for cas_ste in cascade_steps]
    two_Lp = (c_int * len(two_Lp))(*two_Lp)
    delta = [cas_ste[0].delta for cas_ste in cascade_steps]
    delta = (c_double * len(delta))(*delta)

    if Phi == 0. and Theta == 0. and Psi == 0.:
        return lib.angular_correlation(
            theta, phi,
            n_cas_ste,
            two_J, par,
            em_char, two_L,
            em_charp, two_Lp,
            delta
        )

    return lib.angular_correlation_rotated(
        theta, phi,
        n_cas_ste,
        two_J, par,
        em_char, two_L,
        em_charp, two_Lp,
        delta,
        Phi, Theta, Psi
    )    