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

#include <cassert>
#include <stdexcept>

#include "AngularCorrelation.hh"
#include "State.hh"
#include "Transition.hh"

using std::invalid_argument;

/**
 * \brief Test the input validation of the AngularCorrelation class.
 * 
 * Test by calling the constructor with various invalid inputs.
 * The basic example is a
 * 
 * 0^+ -> 1^+ -> 0^+
 * 
 * cascade.
 */
int main(){

    bool error_thrown = false;

    // Error: First EM character not given
    try{
        AngularCorrelation ang_corr(
            State(0, positive), 
            {
                {
                    Transition(em_unknown, 2, magnetic, 4, 0.),
                    State(2, negative)
                },
                {
                    Transition(electric, 2, magnetic, 4, 0.),
                    State(0, positive)
                }
            }
        );
    } catch(const std::invalid_argument e) {
        error_thrown = true;
    }

    assert(error_thrown);
    error_thrown = false;

    // Error: Second EM character not given
    try{
        AngularCorrelation ang_corr(
            State(0, positive), 
            {
                {
                    Transition(electric, 2, em_unknown, 4, 0.),
                    State(2, negative)
                },
                {
                    Transition(electric, 2, magnetic, 4, 0.),
                    State(0, positive)
                }
            }
        );
    } catch(const std::invalid_argument e) {
        error_thrown = true;
    }

    assert(error_thrown);
    error_thrown = false;

    // Error: EM character given, but first spin missing
    try{
        AngularCorrelation ang_corr(
            State(0, parity_unknown), 
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
        );
    } catch(const std::invalid_argument e) {
        error_thrown = true;
    }

    assert(error_thrown);
    error_thrown = false;

    // Error: EM character given, but second spin missing
    try{
        AngularCorrelation ang_corr(
            State(0, positive), 
            {
                {
                    Transition(electric, 2, magnetic, 4, 0.),
                    State(2, parity_unknown)
                },
                {
                    Transition(electric, 2, magnetic, 4, 0.),
                    State(0, positive)
                }
            }
        );
    } catch(const std::invalid_argument e) {
        error_thrown = true;
    }

    assert(error_thrown);
    error_thrown = false;

    // Error: Triangle inequality violated
    try{
        AngularCorrelation ang_corr(
            State(0, positive), 
            {
                {
                    Transition(magnetic, 4, electric, 6, 0.),
                    State(2, negative)
                },
                {
                    Transition(electric, 2, magnetic, 4, 0.),
                    State(0, positive)
                }
            }
        );
    } catch(const std::invalid_argument e) {
        error_thrown = true;
    }

    assert(error_thrown);
    error_thrown = false;

    // Not an error: Triangle inequality fulfilled by second transition
    try{
        AngularCorrelation ang_corr(
            State(0, positive), 
            {
                {
                    Transition(electric, 10, electric, 2, 0.),
                    State(2, negative)
                },
                {
                    Transition(electric, 2, magnetic, 4, 0.),
                    State(0, positive)
                }
            }
        );
    } catch(const std::invalid_argument e) {
        error_thrown = true;
    }

    assert(!error_thrown);

    // Error: First electromagnetic character wrong
    try{
        AngularCorrelation ang_corr(
            State(0, positive), 
            {
                {
                    Transition(magnetic, 2, magnetic, 4, 0.),
                    State(2, negative)
                },
                {
                    Transition(electric, 2, magnetic, 4, 0.),
                    State(0, positive)
                }
            }
        );
    } catch(const std::invalid_argument e) {
        error_thrown = true;
    }

    assert(error_thrown);
    error_thrown = false;

    // Error: Second electromagnetic character wrong
    try{
        AngularCorrelation ang_corr(
            State(0, positive), 
            {
                {
                    Transition(electric, 2, electric, 4, 0.),
                    State(2, negative)
                },
                {
                    Transition(electric, 2, magnetic, 4, 0.),
                    State(0, positive)
                }
            }
        );
    } catch(const std::invalid_argument e) {
        error_thrown = true;
    }

    assert(error_thrown);
    error_thrown = false;

    // Error: Mixing of half-integer and integer spins
    try{
        AngularCorrelation ang_corr(
            State(0, positive), 
            {
                {
                    Transition(electric, 2, magnetic, 4, 0.),
                    State(1, negative)
                },
                {
                    Transition(electric, 2, magnetic, 4, 0.),
                    State(0, positive)
                }
            }
        );
    } catch(const std::invalid_argument e) {
        error_thrown = true;
    }


    assert(error_thrown);
    error_thrown = false;
}