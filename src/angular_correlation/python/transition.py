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

def em_str_rep(em, em_variable_symbol='σ'):
    """String representation of an electromagnetic character

    Parameters
    ----------
    em: int
        EM character.
    em_variable_symbol: string
        Symbol to be displayed when the electromagnetic character is not known.

    Returns
    -------
    string
        'E', if em==-1 (electric character), 'M' if em==1 magnetic, and em_variable_symbol if em \
has any other value.
    """
    if em == ELECTRIC:
        return 'E'
    if em == MAGNETIC:
        return 'M'

    return em_variable_symbol 

def multipolarity_str_rep(two_L):
    """String representation of an multipolarity
    
    Parameters
    ----------
    two_L: int
        Two times the multipolarity.

    Returns
    -------
    string
        If two_L is an even number, the string representation of two_L/2 is returned. \
If two_L is an odd number, an error is raised.

    Raises
    ------
    ValueError
        If two_L is not even.
    """
    if two_L % 2 == 1:
        raise ValueError('Odd value given for two_L.')
    return str(int(two_L/2))

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
        ## Secondary EM character.
        self.em_charp = emp
        ## Two times the secondary multipolarity.
        self.two_Lp = t_Lp
        ## Multipole mixing ratio.
        self.delta = de

    def str(self, separator=' ', secondary_in_parentheses=True, always_show_secondary=True, em_variable_symbol='σ'):
        """String representation of an EM transition between nuclear states.

Returns a customizable string representation of the EM transition in the form: 

str(em_char) + str(two_L) + separator + '(' + str(em_charp) + str(two_Lp) + ')'

Here, str(x) is assumed to be a function that creates a string representation of the attribute x.
It does not have to be equal to the actual 'str()' function of python.

Parameters
----------
separator: string
    Separator between the two possible multipolarities.
secondary_in_parentheses: bool
    Determines whether the parentheses around the secondary multipolarity are shown or not. \
(default: True)
always_show_secondary: bool
    If 'no', the separator, the parentheses and the string representation of the secondary \
multipolarity are not shown if delta == 0. \
(default: True)
em_variable_symbol: string
    Symbol to be displayed when the electromagnetic character is not known. \
[default: σ (Greek letter'sigma')]

Returns
-------
string
    String representation.
        """

        str_rep = em_str_rep(self.em_char, em_variable_symbol=em_variable_symbol) + multipolarity_str_rep(self.two_L)

        if (self.delta == 0 or self.delta == 0.) and not always_show_secondary:
            return str_rep

        str_repp = em_str_rep(self.em_charp, em_variable_symbol=em_variable_symbol) + multipolarity_str_rep(self.two_Lp)

        return str_rep + separator + ('(' if secondary_in_parentheses else '') + str_repp + (')' if secondary_in_parentheses else '')

    def __str__(self):
        """'Official' string representation of a transition

Uses the str() function with default arguments.

Returns
-------
string
    String representation.
        """
        return self.str()

    def tex(self, em_variable_symbol=r'\sigma', always_show_secondary=True, show_polarization=True, dollar=True):
        """TeX representation of an EM transition between nuclear states.

Parameters
----------
em_variable_symbol: string
    Symbol to be displayed when the electromagnetic character is not known. \
[default: r'\sigma']
always_show_secondary: bool
    If 'no', the second multipolarity is not shown if delta == 0.
show_polarization: bool
    If True and all multipolarities are given, indicate that the polarization of the photon \
which was absorbed/emitted in this transition is known by printing a vector arrow above the \
first multipolarity.
dollar: bool
    Determines whether the returned string contains the dollar symbols for math expressions in TeX.

Returns
-------
string
    TeX code
        """

        tex = ('$' if dollar else '')
        if (self.delta == 0 or self.delta == 0.) and not always_show_secondary:
            tex += r'\left('
        else:
            tex += r' \genfrac{(}{)}{0}{}{'

        tex_rep = em_str_rep(self.em_char, em_variable_symbol=em_variable_symbol) + ' ' + multipolarity_str_rep(self.two_L)

        if (EM_UNKNOWN not in (self.em_char, self.em_charp)) and show_polarization:
            tex_rep = r'\overrightarrow{' + tex_rep + '}'

        tex += tex_rep

        if (self.delta == 0 or self.delta == 0.) and not always_show_secondary:
            tex += r'\right)'
            return tex + ('$' if dollar else '')

        tex_repp = em_str_rep(self.em_charp, em_variable_symbol=em_variable_symbol) + ' ' + multipolarity_str_rep(self.two_Lp)

        return tex + '}{' + tex_repp + '}' + ('$' if dollar else '')