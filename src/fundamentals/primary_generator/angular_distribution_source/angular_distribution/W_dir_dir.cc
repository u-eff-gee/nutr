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

#include <algorithm>
#include <cmath>
#include <gsl/gsl_sf.h>

#include "W_dir_dir.hh"

using std::min;

W_dir_dir::W_dir_dir(const State &ini_state, const Transition &ini_to_int, const State &int_state, const Transition &int_to_fin, const State &fin_state):
initial_state(ini_state), initial_to_intermediate(ini_to_int),
intermediate_state(int_state), intermediate_to_final(int_to_fin),
final_state(fin_state), nu_max(0)
{
	two_nu_max = get_two_nu_max(ini_to_int, int_state, int_to_fin);
	nu_max = two_nu_max/2;
	av_prod_cache = get_av_products(two_nu_max, ini_state, ini_to_int, int_state, int_to_fin, fin_state);
}

double W_dir_dir::operator()(const double theta){

	double sum_over_nu{0.};

	for(int i = 0; i <= nu_max/2; ++i){
		sum_over_nu += av_prod_cache[i]*gsl_sf_legendre_Pl(2*i, cos(theta));
	}

	return sum_over_nu;
}

int W_dir_dir::get_two_nu_max(const Transition &ini_to_int, const State &int_state, const Transition &int_to_fin) const {
	
	return 
	2*min(int_state.two_J, 
		min(
			min(ini_to_int.two_L, ini_to_int.two_Lp),
			min(int_to_fin.two_L, int_to_fin.two_Lp)
		)
	);
}

vector<double> W_dir_dir::get_av_products(const int two_nu_max, const State &ini_state, const Transition &ini_to_int, const State &int_state, const Transition &int_to_fin, const State &fin_state) const {

	vector<double> av_products;
	
	for(int two_nu = 0; two_nu <= two_nu_max; two_nu += 4){
		av_products.push_back(
			av_coef(two_nu, ini_to_int.two_L, ini_to_int.two_Lp,
			ini_state.two_J, int_state.two_J, ini_to_int.delta)
			*av_coef(two_nu, ini_to_int.two_L, ini_to_int.two_Lp,
			ini_state.two_J, int_state.two_J, ini_to_int.delta)
			);
	}

	return av_products;
}