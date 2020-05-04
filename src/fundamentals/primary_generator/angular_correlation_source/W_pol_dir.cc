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

W_pol_dir::W_pol_dir(const State &ini_sta, const vector<pair<Transition, State>> cas_ste):
W_gamma_gamma(ini_sta, cas_ste), av_coef(AvCoefficient()), alphav_coef(AlphavCoefficient()), w_dir_dir(W_dir_dir(ini_sta, cas_ste))
{
	two_nu_max = w_dir_dir.get_two_nu_max();
	nu_max = two_nu_max/2;
	expansion_coefficients = calculate_expansion_coefficients();
	normalization_factor = w_dir_dir.get_normalization_factor();

}

double W_pol_dir::operator()(const double theta, const double phi) const {

	double sum_over_nu{0.};

	for(int i = 1; i <= nu_max/2; ++i){
		sum_over_nu += expansion_coefficients[i-1]*gsl_sf_legendre_Plm(2*i, 2, cos(theta));
	}

	int polarization_sign = 1;
	if(cascade_steps[0].first.em_charp == magnetic){
		polarization_sign = -1;
	}

	return w_dir_dir(theta)+polarization_sign*cos(2.*phi)*sum_over_nu*w_dir_dir.get_normalization_factor();
}

vector<double> W_pol_dir::calculate_expansion_coefficients() const {

	vector<double> exp_coef_alphav_Av = calculate_expansion_coefficients_alphav_Av();

	if(n_cascade_steps > 2){
		vector<double> exp_coef_Uv = w_dir_dir.calculate_expansion_coefficients_Uv();
		vector<double> exp_coef(exp_coef_Uv.size(), 0.);

		for(size_t i = 0; i < exp_coef_Uv.size(); ++i){
			exp_coef[i] = exp_coef_alphav_Av[i]*exp_coef_Uv[i];
		}

		return exp_coef;
	}

	return exp_coef_alphav_Av;
}

vector<double> W_pol_dir::calculate_expansion_coefficients_alphav_Av() const {

	vector<double> exp_coef;
	
	for(int two_nu = 4; two_nu <= two_nu_max; two_nu += 4){
		exp_coef.push_back(
			alphav_coef(two_nu, 
				cascade_steps[0].first.two_L, cascade_steps[0].first.two_Lp,
				initial_state.two_J, cascade_steps[0].second.two_J, 
				cascade_steps[0].first.delta)
			*av_coef(two_nu,
				cascade_steps[n_cascade_steps-1].first.two_L, cascade_steps[n_cascade_steps-1].first.two_Lp,
				cascade_steps[n_cascade_steps-1].second.two_J, cascade_steps[n_cascade_steps-2].second.two_J, 
				cascade_steps[n_cascade_steps-1].first.delta)
			);
	}

	return exp_coef;
}