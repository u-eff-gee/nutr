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
 
## @package state
# Module for the representation of a nuclear state
#
# Besides the State class, defines three constants for the accepted values of the parity quantum 
# number.

## Parity: unknown
PARITY_UNKNOWN = 0
## Parity: positive
POSITIVE = 1
## Parity: negative
NEGATIVE = -1


def parity_str_rep(parity, parity_variable_symbol='±'):
    """String representation of a parity quantum number
    
    Parameters
    ----------
    parity: int
        Parity quantum number, which may be 1 (positive parity), -1 (negative), or 0 (parity unknown).
    parity_variable_symbol: string
        Symbol to be displayed when the parity is not known (default: ±).

    Returns
    -------
    string
        '+', if positive parity, '-' if negative parity, and parity_variable_symbol otherwise.

    """
    if parity == POSITIVE:
        return '+'
    if parity == NEGATIVE:
        return '-'

    return parity_variable_symbol

def spin_str_rep(two_J):
    """String representation of an angular momentum ('spin') quantum number
    
    Parameters
    ----------
    two_J: int
        Two times the angular momentum quantum number in units of the reduced Planck constant.
    
    Returns
    -------
    string
        If two_J is an even number, the string representation of two_J/2 is returned. \
If two_J is an odd number 'two_J/2' is returned.
    """
    if two_J % 2 == 0:
        return str(int(two_J/2))

    return str(two_J) + '/2'

def spin_tex_rep(two_J, dollar=False):
    """Tex representation of the angular momentum quantum number of an excited state.
    
    Parameters
    ----------
    two_J: int
        Two times the angular momentum quantum number in units of the reduced Planck constant.
    dollar: bool
        Determines whether the returned string contains the dollar symbols for math expressions in TeX (default: False).
    
    Returns
    -------
    string
        If two_J is an even number, the string representation of two_J/2 is returned. \
If two_J is an odd number '\frac{two_J}{2}' or '$\frac{two_J}{2}$' is returned.    
    """

    tex = ('$' if dollar else '')

    if two_J % 2 == 0:
        return tex + str(int(two_J/2)) + ('$' if dollar else '')

    return tex + r'\frac{' + str(two_J) + '}{2}' + ('$' if dollar else '')

class State:
    """Class to store properties of a nuclear state

    Attributes
    ----------
    two_J: int
        Two times the angular momentum quantum number in units of the reduced Planck constant.
    parity: int
        Parity quantum number, which may be 1 (positive parity), -1 (negative), or 0 (parity unknown).
    """
    def __init__(self, t_J, par):
        """Constructor
        
        Parameters
        ----------
        t_J: int
            Two times the angular momentum quantum number in units of the reduced Planck constant.
        par: int
            Parity quantum number, which may be 1 (positive parity), -1 (negative), or 0 (parity unknown).
        """

        ## Two times the angular momentum quantum number
        self.two_J = t_J 
        ## Parity quantum number
        self.parity = par

    def __str__(self):
        """String representation of a nuclear state
        
        Returns
        -------
        string
            String representation of the state in the form '(two_J/2)^parity'.
For more information, see the spin_str_rep and parity_str_rep functions.
        """
        if self.parity != PARITY_UNKNOWN:
            return spin_str_rep(self.two_J) + '^' + parity_str_rep(self.parity)
        
        return spin_str_rep(self.two_J)

    def tex(self, parity_variable_symbol=r'\pm', dollar=True):
        """TeX representation of a nuclear state.

Parameters
----------
parity_variable_symbol: string
    Symbol to be displayed when the parity is not known (default: '\pm').
dollar: bool
    Determines whether the returned string contains the dollar symbols for math expressions in TeX (default: True).

Returns
-------
string
    TeX code
        """

        return ('$' if dollar else '') + spin_tex_rep(self.two_J) + '^{' + parity_str_rep(self.parity, parity_variable_symbol=parity_variable_symbol) + '}' + ('$' if dollar else '')
