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

#include <cmath>

#include "W_pol_dir.hh"

using std::min;

W_pol_dir::W_pol_dir(const State &ini_state, const Transition &ini_to_int, const State &int_state, const Transition &int_to_fin, const State &fin_state):
initial_state(ini_state), initial_to_intermediate(ini_to_int),
intermediate_state(int_state), intermediate_to_final(int_to_fin),
final_state(fin_state), w_dir_dir(W_dir_dir(ini_state, ini_to_int, int_state, int_to_fin, fin_state))
{}

double W_pol_dir::operator()(const double theta, const double phi){

	return 0;
}