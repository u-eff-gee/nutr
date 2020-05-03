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

#include <gsl/gsl_math.h>

#include "AngularCorrelation.hh"
#include "State.hh"
#include "TestUtilities.hh"
#include "Transition.hh"
#include "W_dir_dir.hh"
#include "W_pol_dir.hh"

/**
 * Test whether AngularCorrelation maps to the correct correlation functions and whether the 
 * rotation works.
 * 
 * The rotation is tested by comparing correlations with polarized incident gamma rays for
 * different electromagnetic (EM) characters of the first transition.
 * The difference in the EM character leads to a rotation of the distribution by \f$\pi / 2\f$ around the z axis.
 */
int main(){

    const double epsilon = 1e-8;

    // Test unpolarized angular correlation
    AngularCorrelation ang_corr_0_1_0{
		State(0, parity_unknown), 
		{
			{
                Transition(em_unknown, 2, em_unknown, 4, 0.),
                State(2, negative)
            }, 
			{
                Transition(em_unknown, 2, em_unknown, 4, 0.),
                State(0, parity_unknown)
            }
		}
    };

    W_dir_dir w_dir_dir_0_1_0{
		State(0, positive), 
		{
			{
                Transition(electric, 2, magnetic, 4, 0.),
                State(2, negative)
            }, 
			{
                Transition(electric, 2, magnetic, 4, 0.),
                State(0, positive)
            }
		}
    };

	for(double theta = 0.; theta < M_PI; theta += 0.5){
		for(double phi = 0.; phi < M_2_PI; phi += 0.5){

			test_numerical_equality<double>(ang_corr_0_1_0(theta, phi), w_dir_dir_0_1_0(theta, phi), epsilon);

		}
	}

    // Test polarized angular correlation and rotation
    AngularCorrelation ang_corr_0p_1p_0p{
		State(0, positive), 
		{
			{
                Transition(magnetic, 2, electric, 4, 0.),
                State(2, positive)
            }, 
			{
                Transition(magnetic, 2, electric, 4, 0.),
                State(0, positive)
            }
		}
    };

    W_pol_dir w_pol_dir_0p_1p_0p{
		State(0, positive), 
		{
			{
                Transition(magnetic, 2, electric, 4, 0.),
                State(2, positive)
            }, 
			{
                Transition(magnetic, 2, electric, 4, 0.),
                State(0, positive)
            }
		}
    };

    W_pol_dir w_pol_dir_0p_1m_0p{
		State(0, positive), 
		{
			{
                Transition(electric, 2, magnetic, 4, 0.),
                State(2, negative)
            }, 
			{
                Transition(electric, 2, magnetic, 4, 0.),
                State(0, positive)
            }
		}
    };

	for(double theta = 0.; theta < M_PI; theta += 0.5){
		for(double phi = 0.; phi < M_2_PI; phi += 0.5){

			test_numerical_equality<double>(ang_corr_0p_1p_0p(theta, phi), w_pol_dir_0p_1p_0p(theta, phi), epsilon);
			test_numerical_equality<double>(ang_corr_0p_1p_0p(theta, phi, {M_PI_2, 0., 0.}), w_pol_dir_0p_1m_0p(theta, phi), epsilon);

		}
	}
}