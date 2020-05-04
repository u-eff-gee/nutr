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

#include <stdexcept>
#include <string>

#include "AngularCorrelation.hh"
#include "TestUtilities.hh"
#include "W_dir_dir.hh"
#include "W_pol_dir.hh"

using std::invalid_argument;
using std::to_string;

AngularCorrelation::AngularCorrelation(const State ini_sta, const vector<pair<Transition, State>> cas_ste):
    euler_angle_rotation(EulerAngleRotation()), w_gamma_gamma(nullptr)
{
    check_cascade(ini_sta, cas_ste);

    if(cas_ste[0].first.em_char == em_unknown){
        w_gamma_gamma = std::make_unique<W_dir_dir>(ini_sta, cas_ste);
    } else{
        w_gamma_gamma = std::make_unique<W_pol_dir>(ini_sta, cas_ste);
    }
}

double AngularCorrelation::operator()(const double theta, const double phi) const {
    return w_gamma_gamma->operator()(theta, phi);
}

double AngularCorrelation::operator()(const double theta, const double phi, const array<double, 3> euler_angles) const {
    array<double, 2> thetap_phip = euler_angle_rotation.rotate_back(array<double, 2>{theta, phi}, euler_angles);

    return (*this)(thetap_phip[0], thetap_phip[1]);
}

void AngularCorrelation::check_cascade(const State ini_sta, const vector<pair<Transition, State>> cas_ste) const {

    if(cas_ste.size() < 2){
        throw invalid_argument("Cascade must have at least two transition - state pairs.");
    }

    check_triangle_inequalities(ini_sta, cas_ste);

    check_em_transitions(ini_sta, cas_ste);

}

void AngularCorrelation::check_triangle_inequalities(const State ini_sta, const vector<pair<Transition, State>> cas_ste) const {
    
    if(
        !fulfils_triangle_inequality<int>(ini_sta.two_J, cas_ste[0].second.two_J, cas_ste[0].first.two_L) && !fulfils_triangle_inequality<int>(ini_sta.two_J, cas_ste[0].second.two_J, cas_ste[0].first.two_Lp)
    ){
        throw invalid_argument("Triangle inequality selection rule not fulfilled for any multipolarity of transition #1: " + cas_ste[0].first.str_rep(ini_sta, cas_ste[0].second));
    }

    for(size_t i = 1; i < cas_ste.size(); ++i){
        if(
            !fulfils_triangle_inequality<int>(cas_ste[i-1].second.two_J, cas_ste[i].second.two_J, cas_ste[i].first.two_L) && !fulfils_triangle_inequality<int>(cas_ste[i-1].second.two_J, cas_ste[i].second.two_J, cas_ste[i].first.two_Lp)
        ){
            throw invalid_argument("Triangle inequality selection rule not fulfilled for any multipolarity of transition #" + cas_ste[i].first.str_rep(cas_ste[i-1].second, cas_ste[i].second));
        }
    }
}

void AngularCorrelation::check_em_transitions(const State ini_sta, const vector<pair<Transition, State>> cas_ste) const {

    if(ini_sta.parity != parity_unknown && cas_ste[0].second.parity != parity_unknown){
        if(cas_ste[0].first.em_char != em_unknown){
            if(!valid_em_character(ini_sta.parity, cas_ste[0].second.parity, cas_ste[0].first.two_L, cas_ste[0].first.em_char)){
                throw invalid_argument("Incorrect electromagnetic character '" + Transition::em_str_rep(cas_ste[0].first.em_char) + "' for transition #1: " + cas_ste[0].first.str_rep(ini_sta, cas_ste[0].second));
            }

            if(cas_ste[0].first.em_charp != em_unknown){
                if(!valid_em_character(ini_sta.parity, cas_ste[0].second.parity, cas_ste[0].first.two_Lp, cas_ste[0].first.em_charp)){
                    throw invalid_argument("Incorrect electromagnetic character '" + Transition::em_str_rep(cas_ste[0].first.em_charp) + "' for transition #1: " + cas_ste[0].first.str_rep(ini_sta, cas_ste[0].second));
                }
            } else{
                throw invalid_argument("Only one electromagnetic character defined for transition #1: " + cas_ste[0].first.str_rep(ini_sta, cas_ste[0].second));
            }
        }

        if(cas_ste[0].first.em_char == em_unknown && cas_ste[0].first.em_charp != em_unknown){
            throw invalid_argument("Only one electromagnetic character defined for transition #1: " + cas_ste[0].first.str_rep(ini_sta, cas_ste[0].second));        
        }
    } else if(cas_ste[0].first.em_char != em_unknown || cas_ste[0].first.em_charp != em_unknown){
        throw invalid_argument("Electromagnetic character defined, but one or both parities missing for transition #1: " + cas_ste[0].first.str_rep(ini_sta, cas_ste[0].second));        
    }

    for(size_t i = 1; i < cas_ste.size(); ++i){
        if(cas_ste[i-1].second.parity != parity_unknown && cas_ste[i].second.parity != parity_unknown){
            if(cas_ste[i].first.em_char != em_unknown){    
                if(!valid_em_character(cas_ste[i-1].second.parity, cas_ste[i].second.parity, cas_ste[i].first.two_L, cas_ste[i].first.em_char)){
                    throw invalid_argument("Incorrect electromagnetic character '" + Transition::em_str_rep(cas_ste[0].first.em_char) + "' for transition #" + to_string(i+1) + ": " + cas_ste[i].first.str_rep(cas_ste[i-1].second, cas_ste[i].second));

                }
            }

            if(cas_ste[i].first.em_charp != em_unknown){    
                if(!valid_em_character(cas_ste[i-1].second.parity, cas_ste[i].second.parity, cas_ste[i].first.two_Lp, cas_ste[i].first.em_charp)){
                    throw invalid_argument("Incorrect electromagnetic character '" + Transition::em_str_rep(cas_ste[0].first.em_charp) + "' for transition #" + to_string(i+1) + ": " + cas_ste[i].first.str_rep(cas_ste[i-1].second, cas_ste[i].second));

                }
            } else{
                throw invalid_argument("Only one electromagnetic character defined for transition #"
                + to_string(i+1) + ": " + cas_ste[i].first.str_rep(cas_ste[i-1].second, cas_ste[i].second)
                );
            }

            if(cas_ste[i].first.em_char == em_unknown && cas_ste[i].first.em_charp != em_unknown){
                throw invalid_argument("Only one electromagnetic character defined for transition #" + to_string(i+1) + ": " + cas_ste[i].first.str_rep(cas_ste[i-1].second, cas_ste[i].second));        
            }
        } else if(cas_ste[i].first.em_char != em_unknown || cas_ste[i].first.em_charp != em_unknown){
            throw invalid_argument("Electromagnetic character defined, but one or both parities missing for transition #" + to_string(i+1) + ": " + cas_ste[i].first.str_rep(cas_ste[i-1].second, cas_ste[i].second));
        }        
    }
}

bool AngularCorrelation::valid_em_character(const Parity p0, const Parity p1, const int two_L, const EMCharacter em) const {

    if(p0 == p1){
        if((two_L/2) % 2 == 0){
            if(em != electric){
                return false;
            }
        } else if(em != magnetic){
            return false;
        }

        return true;
    }

    if((two_L/2) % 2 == 0){
        if(em != magnetic){
            return false;
        }
    } else if(em != electric){
        return false;
    }

    return true;

}

extern "C" double angular_correlation(const double theta, const double phi, const size_t n_cas_ste, int* two_J, char* par, int* em_char, int* two_L, char* em_charp, int* two_Lp, double* delta){

    State initial_state{two_J[0], (Parity) par[0]};
    vector<pair<Transition, State>> cascade_steps;

    for(size_t i = 0; i < n_cas_ste; ++i){
        cascade_steps.push_back(
            {
                Transition{(EMCharacter) em_char[i], two_L[i], (EMCharacter) em_charp[i], two_Lp[i], delta[i]},
                State{two_J[i+1], (Parity) par[i+1]}
            }
        );
    }

    AngularCorrelation ang_corr(initial_state, cascade_steps);

    return ang_corr(theta, phi);
}