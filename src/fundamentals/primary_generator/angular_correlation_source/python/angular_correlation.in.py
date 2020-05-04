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

from ctypes import cdll, c_double, c_int, c_size_t, POINTER

from state import *
from transition import *

lib = cdll.LoadLibrary('@PROJECT_BINARY_DIR@/src/fundamentals/primary_generator/angular_correlation_source/libangular_correlation.so')

lib.angular_correlation.restype = c_double
lib.angular_correlation.argtypes = [
    c_double,          # Polar angle theta
    c_double,          # Azimuthal angle phi
    c_size_t,          # Number of cascade steps
    POINTER(c_int),    # Angular momenta
    POINTER(c_int),    # Parities
    POINTER(c_int),    # EM characters
    POINTER(c_int),    # Multipolarities
    POINTER(c_int),    # Alternative EM characters
    POINTER(c_int),    # Alternative multipolarities
    POINTER(c_double), # Multipole mixing ratios
]

def angular_correlation(theta, phi, 
    initial_state = State(0, POSITIVE), 
    cascade_steps = [
        [Transition(ELECTRIC, 2, MAGNETIC, 4, 0.), State(2, NEGATIVE)],
        [Transition(EM_UNKNOWN, 2, EM_UNKNOWN, 4, 0.), State(0, PARITY_UNKNOWN)],
    ]
):

    n_cas_ste = len(cascade_steps)
    two_J = [cas_ste[1].two_J for cas_ste in cascade_steps]
    two_J.insert(0, initial_state.two_J)
    two_J = (c_int * len(two_J))(*two_J)
    par = [cas_ste[1].parity for cas_ste in cascade_steps]
    par.insert(0, initial_state.parity)
    par = (c_int * len(par))(*par)

    em_char = [cas_ste[0].em_char for cas_ste in cascade_steps]
    em_char = (c_int * len(em_char))(*em_char)
    two_L = [cas_ste[0].two_L for cas_ste in cascade_steps]
    two_L = (c_int * len(two_L))(*two_L)
    em_charp = [cas_ste[0].em_charp for cas_ste in cascade_steps]
    em_charp = (c_int * len(em_charp))(*em_charp)
    two_Lp = [cas_ste[0].two_Lp for cas_ste in cascade_steps]
    two_Lp = (c_int * len(two_Lp))(*two_Lp)
    delta = [cas_ste[0].delta for cas_ste in cascade_steps]
    delta = (c_double * len(delta))(*delta)

    return lib.angular_correlation(
        theta, phi,
        n_cas_ste,
        two_J, par,
        em_char, two_L,
        em_charp, two_Lp,
        delta
    )