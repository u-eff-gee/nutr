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

## @package transition
# Module for the representation of an electromagnetic (EM) transition.
#
# Besides the Transition class, defines three constants for the # accepted values of the EM character.

## EM character: electric
ELECTRIC = -1
## EM character: unknown
EM_UNKNOWN = 0
## EM character: magnetic
MAGNETIC = 1

class Transition:
    """Class to store properties of an EM transition between nuclear states.

    The transition can have two different multipolarities with their associated EM
    character, whose relative intensity is given by the multipole mixing ratio.

    Attributes
    ----------
    em_char: int
        Primary EM character.
    two_L: int
        Two times the primary multipolarity.
    em_charp: int
        Secondary EM character.
    two_Lp: int
        Two times the secondary multipolarity.
    delta: float
        Multipole mixing ratio
    """
    def __init__(self, em, t_L, emp, t_Lp, de):
        """Constructor
        
        Parameters
        ----------
        em: int
            Primary EM character.
        t_L: int
            Two times the primary multipolarity.
        emp: int
            Secondary EM character.
        t_Lp: int
            Two times the secondary multipolarity.
        de: float
            Multipole mixing ratio
        """

        ## Primary EM character.
        self.em_char = em
        ## Two times the primary multipolarity.
        self.two_L = t_L
        ## Primary EM character.
        self.em_charp = emp
        ## Two times the primary multipolarity.
        self.two_Lp = t_Lp
        ## Multipole mixing ratio.
        self.delta = de