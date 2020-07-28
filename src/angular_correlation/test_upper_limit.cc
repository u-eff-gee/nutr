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

#include <array>
// #include <iostream>
#include <vector>

using std::array;
// using std::cout;
// using std::endl;
using std::vector;

#include <gsl/gsl_sf.h>

#include "SpherePointSampler.hh"
#include "State.hh"
#include "Transition.hh"
#include "TestUtilities.hh"
#include "W_dir_dir.hh"
#include "W_gamma_gamma.hh"
#include "W_pol_dir.hh"

vector<W_gamma_gamma*> ang_corrs {
    // // 0 -> 1 -> 0
    new W_dir_dir(
        State(0, parity_unknown), 
        {
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(2, parity_unknown)},
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(0, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(0, positive), 
        {
            {Transition(magnetic, 2, electric, 4, 0.), State(2, positive)},
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(0, parity_unknown)}
        }
    ),

    // 0 -> 2 -> 0
    new W_dir_dir(
        State(0, parity_unknown), 
        {
            {Transition(em_unknown, 4, em_unknown, 6, 0.), State(4, parity_unknown)},
            {Transition(em_unknown, 4, em_unknown, 6, 0.), State(0, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(0, positive), 
        {
            {Transition(electric, 4, magnetic, 6, 0.), State(4, positive)},
            {Transition(em_unknown, 4, em_unknown, 6, 0.), State(0, parity_unknown)}
        }
    ),

    // 0 -> 1 -> 1
    new W_dir_dir(
        State(0, parity_unknown), 
        {
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(2, parity_unknown)},
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(2, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(0, positive), 
        {
            {Transition(magnetic, 2, electric, 4, 0.), State(2, positive)}, 
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(2, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(0, positive), 
        {
            {Transition(magnetic, 2, electric, 4, 0.), State(2, positive)}, 
            {Transition(em_unknown, 2, em_unknown, 4, 1.), State(2, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(0, positive), 
        {
            {Transition(magnetic, 2, electric, 4, 0.), State(2, positive)}, 
            {Transition(em_unknown, 2, em_unknown, 4, -1.), State(2, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(0, positive), 
        {
            {Transition(magnetic, 2, electric, 4, 0.), State(2, positive)}, 
            {Transition(em_unknown, 2, em_unknown, 4, 100.), State(2, parity_unknown)}
        }
    ),

    // 0 -> 1 -> 2
    new W_dir_dir(
        State(0, parity_unknown), 
        {
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(2, parity_unknown)},
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(4, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(0, positive), 
        {
            {Transition(magnetic, 2, electric, 4, 0.), State(2, positive)}, 
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(4, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(0, positive), 
        {
            {Transition(magnetic, 2, electric, 4, 0.), State(2, positive)}, 
            {Transition(em_unknown, 2, em_unknown, 4, 1.), State(4, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(0, positive), 
        {
            {Transition(magnetic, 2, electric, 4, 0.), State(2, positive)}, 
            {Transition(em_unknown, 2, em_unknown, 4, -1.), State(4, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(0, positive), 
        {
            {Transition(magnetic, 2, electric, 4, 0.), State(2, positive)}, 
            {Transition(em_unknown, 2, em_unknown, 4, 100.), State(4, parity_unknown)}
        }
    ),

    // 0 -> 1 -> 3
    new W_dir_dir(
        State(0, parity_unknown), 
        {
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(2, parity_unknown)},
            {Transition(em_unknown, 4, em_unknown, 6, 0.), State(6, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(0, positive), 
        {
            {Transition(magnetic, 2, electric, 4, 0.), State(2, positive)}, 
            {Transition(em_unknown, 4, em_unknown, 6, 0.), State(6, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(0, positive), 
        {
            {Transition(magnetic, 2, electric, 4, 0.), State(2, positive)}, 
            {Transition(em_unknown, 4, em_unknown, 6, 1.), State(6, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(0, positive), 
        {
            {Transition(magnetic, 2, electric, 4, 0.), State(2, positive)}, 
            {Transition(em_unknown, 4, em_unknown, 6, -1.), State(6, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(0, positive), 
        {
            {Transition(magnetic, 2, electric, 4, 0.), State(2, positive)}, 
            {Transition(em_unknown, 4, em_unknown, 6, 100.), State(6, parity_unknown)}
        }
    ),

    // 0 -> 2 -> 1
    new W_dir_dir(
        State(0, parity_unknown), 
        {
            {Transition(em_unknown, 4, em_unknown, 6, 0.), State(4, parity_unknown)},
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(2, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(0, positive), 
        {
            {Transition(magnetic, 4, electric, 6, 0.), State(4, positive)}, 
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(2, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(0, positive), 
        {
            {Transition(magnetic, 4, electric, 6, 0.), State(4, positive)}, 
            {Transition(em_unknown, 2, em_unknown, 4, 1.), State(2, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(0, positive), 
        {
            {Transition(magnetic, 4, electric, 6, 0.), State(4, positive)}, 
            {Transition(em_unknown, 2, em_unknown, 4, -1.), State(2, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(0, positive), 
        {
            {Transition(magnetic, 4, electric, 6, 0.), State(4, positive)}, 
            {Transition(em_unknown, 2, em_unknown, 4, 100.), State(2, parity_unknown)}
        }
    ),

    // 0 -> 2 -> 2
    new W_dir_dir(
        State(0, parity_unknown), 
        {
            {Transition(em_unknown, 4, em_unknown, 6, 0.), State(4, parity_unknown)},
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(4, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(0, positive), 
        {
            {Transition(magnetic, 4, electric, 6, 0.), State(4, positive)}, 
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(4, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(0, positive), 
        {
            {Transition(magnetic, 4, electric, 6, 0.), State(4, positive)}, 
            {Transition(em_unknown, 2, em_unknown, 4, 1.), State(4, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(0, positive), 
        {
            {Transition(magnetic, 4, electric, 6, 0.), State(4, positive)}, 
            {Transition(em_unknown, 2, em_unknown, 4, -1.), State(4, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(0, positive), 
        {
            {Transition(magnetic, 4, electric, 6, 0.), State(4, positive)}, 
            {Transition(em_unknown, 2, em_unknown, 4, 100.), State(4, parity_unknown)}
        }
    ),

    // 0 -> 2 -> 3
    new W_dir_dir(
        State(0, parity_unknown), 
        {
            {Transition(em_unknown, 4, em_unknown, 6, 0.), State(4, parity_unknown)},
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(6, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(0, positive), 
        {
            {Transition(magnetic, 4, electric, 6, 0.), State(4, positive)}, 
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(6, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(0, positive), 
        {
            {Transition(magnetic, 4, electric, 6, 0.), State(4, positive)}, 
            {Transition(em_unknown, 2, em_unknown, 4, 1.), State(6, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(0, positive), 
        {
            {Transition(magnetic, 4, electric, 6, 0.), State(4, positive)}, 
            {Transition(em_unknown, 2, em_unknown, 4, -1.), State(6, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(0, positive), 
        {
            {Transition(magnetic, 4, electric, 6, 0.), State(4, positive)}, 
            {Transition(em_unknown, 2, em_unknown, 4, 100.), State(6, parity_unknown)}
        }
    ),

    // 0 -> 2 -> 4
    new W_dir_dir(
        State(0, parity_unknown), 
        {
            {Transition(em_unknown, 4, em_unknown, 6, 0.), State(4, parity_unknown)},
            {Transition(em_unknown, 4, em_unknown, 6, 0.), State(8, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(0, positive), 
        {
            {Transition(magnetic, 4, electric, 6, 0.), State(4, positive)}, 
            {Transition(em_unknown, 4, em_unknown, 6, 0.), State(8, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(0, positive), 
        {
            {Transition(magnetic, 4, electric, 6, 0.), State(4, positive)}, 
            {Transition(em_unknown, 4, em_unknown, 6, 1.), State(8, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(0, positive), 
        {
            {Transition(magnetic, 4, electric, 6, 0.), State(4, positive)}, 
            {Transition(em_unknown, 4, em_unknown, 6, -1.), State(8, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(0, positive), 
        {
            {Transition(magnetic, 4, electric, 6, 0.), State(4, positive)}, 
            {Transition(em_unknown, 4, em_unknown, 6, 100.), State(8, parity_unknown)}
        }
    ),

    // 1/2 -> 5/2 -> 1/2
    new W_dir_dir(
        State(1, parity_unknown), 
        {
            {Transition(em_unknown, 4, em_unknown, 6, 0.), State(5, parity_unknown)},
            {Transition(em_unknown, 4, em_unknown, 6, 0.), State(1, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(1, positive), 
        {
            {Transition(electric, 4, magnetic, 6, 0.), State(5, positive)},
            {Transition(em_unknown, 4, em_unknown, 6, 0.), State(1, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(1, positive), 
        {
            {Transition(electric, 4, magnetic, 6, 1.), State(5, positive)},
            {Transition(em_unknown, 4, em_unknown, 6, 1.), State(1, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(1, positive), 
        {
            {Transition(electric, 4, magnetic, 6, -1.), State(5, positive)},
            {Transition(em_unknown, 4, em_unknown, 6, -1.), State(1, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(1, positive), 
        {
            {Transition(electric, 4, magnetic, 6, 100.), State(5, positive)},
            {Transition(em_unknown, 4, em_unknown, 6, 100.), State(1, parity_unknown)}
        }
    ),

    // 3/2 -> 3/2 -> 3/2
    new W_dir_dir(
        State(3, parity_unknown), 
        {
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(3, parity_unknown)},
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(3, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(3, positive), 
        {
            {Transition(magnetic, 2, electric, 4, 0.), State(3, positive)},
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(3, positive)}
        }
    ),
    new W_pol_dir(
        State(3, positive), 
        {
            {Transition(magnetic, 2, electric, 4, 1.), State(3, positive)},
            {Transition(em_unknown, 2, em_unknown, 4, 1.), State(3, positive)}
        }
    ),
    new W_pol_dir(
        State(3, positive), 
        {
            {Transition(magnetic, 2, electric, 4, -1.), State(3, positive)},
            {Transition(em_unknown, 2, em_unknown, 4, -1.), State(3, positive)}
        }
    ),
    new W_pol_dir(
        State(3, positive), 
        {
            {Transition(magnetic, 2, electric, 4, 100.), State(3, positive)},
            {Transition(em_unknown, 2, em_unknown, 4, 100.), State(3, positive)}
        }
    ),

    // 3/2 -> 5/2 -> 3/2
    new W_dir_dir(
        State(3, parity_unknown), 
        {
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(5, parity_unknown)},
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(3, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(3, positive), 
        {
            {Transition(magnetic, 2, electric, 4, 0.), State(5, positive)},
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(3, positive)}
        }
    ),
    new W_pol_dir(
        State(3, positive), 
        {
            {Transition(magnetic, 2, electric, 4, 1.), State(5, positive)},
            {Transition(em_unknown, 2, em_unknown, 4, 1.), State(3, positive)}
        }
    ),
    new W_pol_dir(
        State(3, positive), 
        {
            {Transition(magnetic, 2, electric, 4, -1.), State(5, positive)},
            {Transition(em_unknown, 2, em_unknown, 4, -1.), State(3, positive)}
        }
    ),
    new W_pol_dir(
        State(3, positive), 
        {
            {Transition(magnetic, 2, electric, 4, 100.), State(5, positive)},
            {Transition(em_unknown, 2, em_unknown, 4, 100.), State(3, positive)}
        }
    ),

    // 3/2 -> 7/2 -> 3/2
    new W_dir_dir(
        State(3, parity_unknown), 
        {
            {Transition(em_unknown, 4, em_unknown, 6, 0.), State(7, parity_unknown)},
            {Transition(em_unknown, 4, em_unknown, 6, 0.), State(3, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(3, positive), 
        {
            {Transition(electric, 4, magnetic, 6, 0.), State(7, positive)},
            {Transition(em_unknown, 4, em_unknown, 6, 0.), State(3, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(3, positive), 
        {
            {Transition(electric, 4, magnetic, 6, 1.), State(7, positive)},
            {Transition(em_unknown, 4, em_unknown, 6, 1.), State(3, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(3, positive), 
        {
            {Transition(electric, 4, magnetic, 6, -1.), State(7, positive)},
            {Transition(em_unknown, 4, em_unknown, 6, -1.), State(3, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(3, positive), 
        {
            {Transition(electric, 4, magnetic, 6, 100.), State(7, positive)},
            {Transition(em_unknown, 4, em_unknown, 6, 100.), State(3, parity_unknown)}
        }
    ),

    // 5/2 -> 3/2 -> 5/2
    new W_dir_dir(
        State(5, parity_unknown), 
        {
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(3, parity_unknown)},
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(5, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(5, positive), 
        {
            {Transition(magnetic, 2, electric, 4, 0.), State(3, positive)},
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(5, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(5, positive), 
        {
            {Transition(magnetic, 2, electric, 4, 1.), State(3, positive)},
            {Transition(em_unknown, 2, em_unknown, 4, 1.), State(5, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(5, positive), 
        {
            {Transition(magnetic, 2, electric, 4, -1.), State(3, positive)},
            {Transition(em_unknown, 2, em_unknown, 4, -1.), State(5, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(5, positive), 
        {
            {Transition(magnetic, 2, electric, 4, 100.), State(3, positive)},
            {Transition(em_unknown, 2, em_unknown, 4, 100.), State(5, parity_unknown)}
        }
    ),

    // 5/2 -> 5/2 -> 5/2
    new W_dir_dir(
        State(5, parity_unknown), 
        {
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(5, parity_unknown)},
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(5, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(5, positive), 
        {
            {Transition(magnetic, 2, electric, 4, 0.), State(5, positive)},
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(5, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(5, positive), 
        {
            {Transition(magnetic, 2, electric, 4, 1.), State(5, positive)},
            {Transition(em_unknown, 2, em_unknown, 4, 1.), State(5, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(5, positive), 
        {
            {Transition(magnetic, 2, electric, 4, -1.), State(5, positive)},
            {Transition(em_unknown, 2, em_unknown, 4, -1.), State(5, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(5, positive), 
        {
            {Transition(magnetic, 2, electric, 4, 100.), State(5, positive)},
            {Transition(em_unknown, 2, em_unknown, 4, 100.), State(5, parity_unknown)}
        }
    ),

    // 5/2 -> 7/2 -> 5/2
    new W_dir_dir(
        State(5, parity_unknown), 
        {
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(7, parity_unknown)},
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(5, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(5, positive), 
        {
            {Transition(magnetic, 2, electric, 4, 0.), State(7, positive)},
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(5, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(5, positive), 
        {
            {Transition(magnetic, 2, electric, 4, 1.), State(7, positive)},
            {Transition(em_unknown, 2, em_unknown, 4, 1.), State(5, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(5, positive), 
        {
            {Transition(magnetic, 2, electric, 4, -1.), State(7, positive)},
            {Transition(em_unknown, 2, em_unknown, 4, -1.), State(5, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(5, positive), 
        {
            {Transition(magnetic, 2, electric, 4, 100.), State(7, positive)},
            {Transition(em_unknown, 2, em_unknown, 4, 100.), State(5, parity_unknown)}
        }
    ),

    // 5/2 -> 9/2 -> 5/2
    new W_dir_dir(
        State(5, parity_unknown), 
        {
            {Transition(em_unknown, 4, em_unknown, 6, 0.), State(9, parity_unknown)},
            {Transition(em_unknown, 4, em_unknown, 6, 0.), State(5, parity_unknown)}
        }
    ),
    new W_dir_dir(
        State(5, positive), 
        {
            {Transition(electric, 4, magnetic, 6, 0.), State(9, positive)},
            {Transition(em_unknown, 4, em_unknown, 6, 0.), State(5, parity_unknown)}
        }
    ),
    new W_dir_dir(
        State(5, positive), 
        {
            {Transition(electric, 4, magnetic, 6, 1.), State(9, positive)},
            {Transition(em_unknown, 4, em_unknown, 6, 1.), State(5, parity_unknown)}
        }
    ),
    new W_dir_dir(
        State(5, positive), 
        {
            {Transition(electric, 4, magnetic, 6, -1.), State(9, positive)},
            {Transition(em_unknown, 4, em_unknown, 6, -1.), State(5, parity_unknown)}
        }
    ),
    new W_dir_dir(
        State(5, positive), 
        {
            {Transition(electric, 4, magnetic, 6, 100.), State(9, positive)},
            {Transition(em_unknown, 4, em_unknown, 6, 100.), State(5, parity_unknown)}
        }
    ),

    // 0 -> 1 -> 1 -> 0
    new W_dir_dir(
        State(0, parity_unknown), 
        {
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(2, parity_unknown)},
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(2, parity_unknown)},
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(0, parity_unknown)}
        }
    ),    
    new W_dir_dir(
        State(0, parity_unknown), 
        {
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(2, parity_unknown)},
            {Transition(em_unknown, 2, em_unknown, 4, 1.), State(2, parity_unknown)},
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(0, parity_unknown)}
        }
    ),
    new W_dir_dir(
        State(0, parity_unknown), 
        {
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(2, parity_unknown)},
            {Transition(em_unknown, 2, em_unknown, 4, 100.), State(2, parity_unknown)},
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(0, parity_unknown)}
        }
    ),

    new W_pol_dir(
        State(0, positive), 
        {
            {Transition(magnetic, 2, electric, 4, 0.), State(2, positive)},
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(2, parity_unknown)},
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(0, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(0, positive), 
        {
            {Transition(magnetic, 2, electric, 4, 0.), State(2, positive)},
            {Transition(em_unknown, 2, em_unknown, 4, 1.), State(2, parity_unknown)},
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(0, parity_unknown)}
        }
    ), 
    new W_pol_dir(
        State(0, positive), 
        {
            {Transition(magnetic, 2, electric, 4, 0.), State(2, positive)},
            {Transition(em_unknown, 2, em_unknown, 4, 100.), State(2, parity_unknown)},
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(0, parity_unknown)}
        }
    ),

    // 0 -> 2 -> 2 -> 0
    new W_dir_dir(
        State(0, parity_unknown), 
        {
            {Transition(em_unknown, 4, em_unknown, 6, 0.), State(4, parity_unknown)},
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(4, parity_unknown)},
            {Transition(em_unknown, 4, em_unknown, 6, 0.), State(0, parity_unknown)}
        }
    ),    
    new W_dir_dir(
        State(0, parity_unknown), 
        {
            {Transition(em_unknown, 4, em_unknown, 6, 0.), State(4, parity_unknown)},
            {Transition(em_unknown, 2, em_unknown, 4, 1.), State(4, parity_unknown)},
            {Transition(em_unknown, 4, em_unknown, 6, 0.), State(0, parity_unknown)}
        }
    ),    
    new W_dir_dir(
        State(0, parity_unknown), 
        {
            {Transition(em_unknown, 4, em_unknown, 6, 0.), State(4, parity_unknown)},
            {Transition(em_unknown, 2, em_unknown, 4, 100.), State(4, parity_unknown)},
            {Transition(em_unknown, 4, em_unknown, 6, 0.), State(0, parity_unknown)}
        }
    ),    

    new W_pol_dir(
        State(0, positive), 
        {
            {Transition(electric, 4, magnetic, 6, 0.), State(4, positive)},
            {Transition(em_unknown, 2, em_unknown, 4, 0.), State(4, parity_unknown)},
            {Transition(em_unknown, 4, em_unknown, 6, 0.), State(0, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(0, positive), 
        {
            {Transition(electric, 4, magnetic, 6, 0.), State(4, positive)},
            {Transition(em_unknown, 2, em_unknown, 4, 1.), State(4, parity_unknown)},
            {Transition(em_unknown, 4, em_unknown, 6, 0.), State(0, parity_unknown)}
        }
    ),
    new W_pol_dir(
        State(0, positive), 
        {
            {Transition(electric, 4, magnetic, 6, 0.), State(4, positive)},
            {Transition(em_unknown, 2, em_unknown, 4, 100.), State(4, parity_unknown)},
            {Transition(em_unknown, 4, em_unknown, 6, 0.), State(0, parity_unknown)}
        }
    ),
};

/**
 * \brief Test upper limits of angular correlations.
 * 
 * Test by sampling values from different angular correlations and comparing them to the
 * upper limit.
 * The almost uniform sampling is performed using the SpherePointSampler.
 */
int main(){

    const SpherePointSampler sph_poi_sam;

    const unsigned int n_sphere_points = 1000;
    array<vector<double>, 2> sphere_points = sph_poi_sam.sample(n_sphere_points);

    double ang_cor_max{0.}, ang_cor_val{0.}, ang_cor_upp_lim{0.};

    for(auto ang_cor : ang_corrs){
        ang_cor_max = 0.;
        ang_cor_upp_lim = ang_cor->get_upper_limit();

        for(size_t i = 0; i < n_sphere_points; ++i){
            ang_cor_val = ang_cor->operator()(sphere_points[0][i], sphere_points[1][i]);
            assert(ang_cor_val <= ang_cor_upp_lim);
            if(ang_cor_val > ang_cor_max){
                ang_cor_max = ang_cor_val;
            }
        }

        // Test output
        // cout << std::scientific << ang_cor_max << " <-> " << ang_cor_upp_lim << " ( " << std::defaultfloat << ang_cor_upp_lim/ang_cor_max*100. << " % ) " << endl;
    }
}