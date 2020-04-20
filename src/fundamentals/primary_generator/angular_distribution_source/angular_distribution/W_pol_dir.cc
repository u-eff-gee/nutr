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

#include <gsl/gsl_sf.h>

#include "W_pol_dir.hh"

using std::min;

W_pol_dir::W_pol_dir(const State &ini_state, const Transition &ini_to_int, const State &int_state, const Transition &int_to_fin, const State &fin_state):
initial_state(ini_state), initial_to_intermediate(ini_to_int),
intermediate_state(int_state), intermediate_to_final(int_to_fin),
final_state(fin_state), alphav_coef(AlphavCoefficient()), av_coef(AvCoefficient()), w_dir_dir(W_dir_dir(ini_state, ini_to_int, int_state, int_to_fin, fin_state))
{
	alphav_av_prod_cache = get_alphav_av_products(w_dir_dir.get_two_nu_max(), ini_state, ini_to_int, int_state, int_to_fin, fin_state);
}

double W_pol_dir::operator()(const double theta, const double phi) const {

	double sum_over_nu{0.};

	for(int i = 1; i <= w_dir_dir.get_nu_max()/2; ++i){
		sum_over_nu += alphav_av_prod_cache[i-1]*gsl_sf_legendre_Plm(2*i, 2, cos(theta));
	}

	int polarization_sign{1};
	if(initial_to_intermediate.em_charp == magnetic){
		polarization_sign = -1;
	}

	return w_dir_dir(theta)+polarization_sign*cos(2.*phi)*sum_over_nu/(1.+initial_to_intermediate.delta*initial_to_intermediate.delta)/(1.+intermediate_to_final.delta*intermediate_to_final.delta);
}

vector<double> W_pol_dir::get_alphav_av_products(const int two_nu_max, const State &ini_state, const Transition &ini_to_int, const State &int_state, const Transition &int_to_fin, const State &fin_state) const {

	vector<double> alphav_av_products;
	
	for(int two_nu = 4; two_nu <= two_nu_max; two_nu += 4){
		alphav_av_products.push_back(
			alphav_coef(two_nu, ini_to_int.two_L, ini_to_int.two_Lp,
			ini_state.two_J, int_state.two_J, ini_to_int.delta)
			*av_coef(two_nu, int_to_fin.two_L, int_to_fin.two_Lp,
			fin_state.two_J, int_state.two_J, int_to_fin.delta)
			);
	}

	return alphav_av_products;
}

extern "C" double w_pol_dir(const double theta, const double phi, const int two_J_ini, const int p_ini, const int em_ini_to_int, const int two_L_ini_to_int, const int emp_ini_to_int, const int two_Lp_ini_to_int, const double delta_ini_to_int, const int two_J_int, const int p_int, const int two_L_int_to_fin, const int two_Lp_int_to_fin, const double delta_int_to_fin, const int two_J_fin, const int p_fin){

	W_pol_dir w_p_d(
			State(two_J_ini, p_ini == 1 ? positive : negative),
			Transition(em_ini_to_int == 1 ? magnetic : electric, two_L_ini_to_int, emp_ini_to_int == 1 ? magnetic : electric, two_Lp_ini_to_int, delta_ini_to_int), 
			State(two_J_int, p_int == 1 ? positive : negative),
			Transition(em_unknown, two_L_int_to_fin, em_unknown, two_Lp_int_to_fin, delta_int_to_fin), 
			State(two_J_fin, p_fin == 1 ? positive : negative));
	return w_p_d(theta, phi);
}