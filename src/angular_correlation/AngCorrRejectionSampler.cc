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

#include "AngCorrRejectionSampler.hh"

AngCorrRejectionSampler::AngCorrRejectionSampler(W_gamma_gamma* w, const int seed, const unsigned int max_tri): 
    SphereRejectionSampler(nullptr, w->get_upper_limit(), seed, max_tri),
    w_gamma_gamma(w)
{};

tuple<unsigned int, double, double> AngCorrRejectionSampler::sample(){

    pair<double, double> theta_phi;
    double dis_val;

    for(unsigned int i = 0; i < max_tries; ++i){

        theta_phi = sample_theta_phi();
        dis_val = uniform_random(random_engine)*distribution_maximum;

        if(dis_val <= w_gamma_gamma->operator()(theta_phi.first, theta_phi.second)){
            return {i+1, theta_phi.first, theta_phi.second};
        }

    }

    return {max_tries, 0., 0.};
};