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
#include <vector>

#include "SpherePointSampler.hh"
#include "TestUtilities.hh"

using std::invalid_argument;
using std::vector;

int main(){

    const unsigned int n = 100;

    SpherePointSampler sph_pt_samp;
    const double c = sph_pt_samp.find_c(n);

	bool error_thrown{false};
	try{
		sph_pt_samp.find_Theta_j(0, n, c);
	} catch(const std::invalid_argument e){
		error_thrown = true;
	}
	assert(error_thrown);
    error_thrown = false;

    double Theta_j = 0.;

    for(unsigned int j = 1; j <= n; ++j){
        Theta_j = sph_pt_samp.find_Theta_j(j, n, c);

        // Test using the defining equation {Eq. (11) in Ref. \cite Koay2011} for Theta_j
        test_numerical_equality<double>(sph_pt_samp.segment_length(Theta_j, c), (2.*j-1)*M_PI/c, 1e-8);
    }

	try{
		sph_pt_samp.find_Theta_j(n+1, n, c);
	} catch(const std::invalid_argument e){
		error_thrown = true;
	}
	assert(error_thrown);    
}