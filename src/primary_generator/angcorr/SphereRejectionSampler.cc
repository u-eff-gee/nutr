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

#include <numeric>
#include <utility>
#include <vector>

using std::accumulate;
using std::get;
using std::vector;

#include <gsl/gsl_math.h>

#include "SphereRejectionSampler.hh"

SphereRejectionSampler::SphereRejectionSampler(function<double(const double, const double)> dis, const double dis_max, const int seed, const unsigned int max_tri):
    distribution(dis),
    distribution_maximum(dis_max),
    max_tries(max_tri)
{
    random_engine = mt19937(seed);
};

tuple<unsigned int, double, double> SphereRejectionSampler::sample(){
    
    pair<double, double> theta_phi;
    double dis_val;

    for(unsigned int i = 0; i < max_tries; ++i){

        theta_phi = sample_theta_phi();
        dis_val = uniform_random(random_engine)*distribution_maximum;

        if(dis_val <= distribution(theta_phi.first, theta_phi.second)){
            return {i+1, theta_phi.first, theta_phi.second};
        }

    }

    return {max_tries, 0., 0.};
}

pair<double, double> SphereRejectionSampler::operator()(){
    tuple<unsigned int, double, double> sampled_theta_phi = sample();

    return {get<1>(sampled_theta_phi), get<2>(sampled_theta_phi)};
}

double SphereRejectionSampler::estimate_efficiency(const unsigned int n_tries){
    vector<unsigned int> required_tries(n_tries);
    
    tuple<unsigned int, double, double> sampled_theta_phi;

    for(unsigned int i = 0; i < n_tries; ++i){
        sampled_theta_phi = sample();
        required_tries[i] = get<0>(sampled_theta_phi);
    }

    return (double) n_tries / (double) accumulate(required_tries.begin(), required_tries.end(), 0);
}

double SphereRejectionSampler::sample_theta(){
    return acos(2.*uniform_random(random_engine)-1.);
}

double SphereRejectionSampler::sample_phi(){
    return 2.*M_PI*uniform_random(random_engine);
}

pair<double, double> SphereRejectionSampler::sample_theta_phi(){
    return {sample_theta(), sample_phi()};
}