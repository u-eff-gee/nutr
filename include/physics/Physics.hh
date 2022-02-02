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

    Copyright (C) 2020-2022 Udo Friman-Gayer
*/

#pragma once

#include "G4VModularPhysicsList.hh"

/**
 * \brief Definition of physics processes
 * 
 * nutr creates a G4VModularPhysicsList, which calls the constructors of several 
 * G4VPhysicsConstructor classes provided by Geant4.
 * A complete overview of all classes derived from G4VPhysicsConstructor can be found in 
 * '$G4_SOURCE/source/physics_lists/constructors', where $G4_SRC is the source directory of Geant4.
 * The Geant4 webpage \cite Geant42020 also provides a physics list guide that introduces all the
 * predefined modular physics lists.
 * 
 * The physics list of nutr aims to include the most precise implementation of all processes that
 * are relevant for low-energy experiments at HI\f$\gamma\f$S.
 * As a matter of fact, nuclear resonance fluorescence is not yet a part of the Geant4's standard
 * physics processes.
 * Packages for the simulation of NRF have been developed by Lakshmanan et al. \cite Lakshmanan2014 and by Vavrek et al. \cite Vavrek2018.
 * The latter is publicly available and has been tested extensively by the authors 
 * \cite Vavrek2019.
 */
class Physics : public G4VModularPhysicsList {

public:
	Physics(); /**< Constructor */

    void SetCuts() override;
};
