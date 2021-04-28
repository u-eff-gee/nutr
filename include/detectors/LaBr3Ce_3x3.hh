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

#include "Detector.hh"

/**
 * \brief Class for a 3 in x 3 in LaBr3:Ce scintillation detector with a dummy case.
 * 
 * This class implements a simplified standard configuration of 3 in x 3 in scintillation 
 * detectors as manufactured by the company Saint Gobian \cite SaintGobain2021.
 * The sensitive crystals are made of cerium-doped lanthanum-bromide (LaBr3:Ce) material.
 * The dimensions used in this implementation are from a previous implementation by B. Löher and 
 * J. Isaak (BI) (crystal, vacuum, and crystal housing) or from a measurement by the author in 
 * 2018 (outer parts).
 * If not indicated otherwise, dimensions were read off from the technical drawings on the SCIONIX 
 * webpage for scintillation crystals with a photomultiplier tube (PMT) and a preamplifier.
 * 	
 * Only the outer dimensions of the housing, but no details about the inside, are known. This 
 * is why all the back parts are simply constructed as hollow aluminium. The housing is 
 * only implemented for aesthetic reasons and can be switched off via the 'use_housing' flag.

 */
class LaBr3Ce_3x3 : public Detector{
    public:
        LaBr3Ce_3x3(G4LogicalVolume *World_Logical, G4String name) : Detector (World_Logical, name){};

		void Construct(G4ThreeVector global_coordinates, G4double theta, G4double phi, G4double dist_from_center, G4double intrinsic_rotation_angle = 0.) override final;
        
};