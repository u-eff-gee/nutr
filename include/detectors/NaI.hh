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

	Copyright (C) 2020, 2021 Udo Friman-Gayer
*/

#pragma once

#include <vector>

#include "G4LogicalVolume.hh"

#include "Detector.hh"

using std::vector;

/**
 * \brief Class for a Saint-Gobain NaI(Tl) detector from a γγ-coincidence spectrometer
 * 
 * Two of these NaI(Tl) detectors were used in 'A low-background γγ-coincidence spectrometer for 
 * radioisotope studies' \cite Tillett2017 \cite Tillett2018.
 * 
 * The implementation was extracted from another Geant4 simulation of the coincidence spectrometer,
 * which was provided to the author by J. Dermigny, one of the authors of the aforementioned
 * publications.
 * The largest parts of the geometry are a cylindrical NaI(Tl) crystal and the aluminium can 
 * around it.
 * Electronics and the photomultiplier tube at the back of the detector are not implemented, but 
 * the detector is mostly filled with air.
 * 
 * Compared to the original implementation, the class has been adapted to the placement- and 
 * sensitive-detector-registration mechanics of 'nutr'.
 * It is now a derived class of 'Detector'.
 * In order to make the previous code compatible, the placement of the mother volume was 
 * generalized, and the construction of the G4PolyCone that represents the aluminium can was
 * adapted to the correct orientation.
 * Furthermore, some comments were removed/edited and code was updated (replace divisions by 2 
 * with multiplications by 0.5, use std::vector instead of C-style arrays, use Geant4-predefined
 * materials instead of building them from scratch ...).
 * Besides the logical volume of the NaI(Tl) crystal, none of the variable names were changed and 
 * all comments were left as they were.
 * 
 * To the author of the present code, it seemed like the topology of both NaI(Tl) detectors in the
 * setup was exactly the same, although the previous code had two separate implementations.
 * 
 * \todo Simplify the code. There are a lot of unnecessary variable names and redundant calculations.
 * \todo Instead of placing the geometry in its own mother volume, place everything in the world 
 * volume to avoid volume overlaps.
 * \todo Adapt to nutr conventions. For example, the factor 0.5 for lengths should be applied 
 * when the constructor of the G4VSolid is called, and not when the variable is defined.
 */
class NaI : public Detector{
	public:
        /**
         * \brief Constructor
         */
		NaI(G4LogicalVolume *World_Logical, G4String name) : Detector (World_Logical, name){};

		void Construct(G4ThreeVector global_coordinates, G4double theta, G4double phi,
			       G4double dist_from_center, G4double intrinsic_rotation_angle = 0.) override; /**< Constructs the geometry */

};