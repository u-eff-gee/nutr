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

    Copyright (C) 2020, 2021 Udo Friman-Gayer
*/

#include "AlphavCoefficient.hh"

AlphavCoefficient::AlphavCoefficient():
f_coef(FCoefficient()), kappa_coef(KappaCoefficient())
{}

double AlphavCoefficient::operator()(const int two_nu, const int two_L, const int two_Lp, const int two_jn, const int two_j, const double delta) const {
	
	return -kappa_coef(two_nu, two_L, two_L)*f_coef(two_nu, two_L, two_L, two_jn, two_j)
		+ 2.*delta*kappa_coef(two_nu, two_L, two_Lp)*f_coef(two_nu, two_L, two_Lp, two_jn, two_j)
		+ delta*delta*kappa_coef(two_nu, two_Lp, two_Lp)*f_coef(two_nu, two_Lp, two_Lp, two_jn, two_j);
}
