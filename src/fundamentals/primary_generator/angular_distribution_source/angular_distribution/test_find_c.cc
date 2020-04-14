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

    const vector<unsigned int> n_points_values { 2, 10, 100, 1000, 10000, 100000, 1000000 };

    SpherePointSampler sph_pt_samp;

	bool error_thrown{false};
	try{
		sph_pt_samp.find_c(1);
	} catch(const std::invalid_argument e){
		error_thrown = true;
	}
	assert(error_thrown);

    double c = 0.;

    for(auto n_points: n_points_values){
        c = sph_pt_samp.find_c(n_points, 1e-8);

        // The fixed-point formula that defines the optimum c {Eq. (8) in Ref. \cite Koay2011}
        // provides a straightforward test.
        test_numerical_equality<double>(c, 2.*n_points*M_PI/sph_pt_samp.segment_length(M_PI, c), 1e-8);
    }


}