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

#include "Detector.hh"

/**
 * \brief Class for a 1.5 in x 1.5 in CeBr3 scintillation detector with PMT and connectors
 * 
 * This class implements a simplified standard configuration of 1.5 in x 1.5 in scintillation 
 * detectors as manufactured by the company SCIONIX \cite SCIONIX2020.
 * The sensitive crystals are made of the low-background (as compared to lanthanum-based 
 * scintillators) cerium-bromide (CeBr3) material.
 * If not indicated otherwise, dimensions were read off from the technical drawings on the SCIONIX 
 *  webpage for scintillation crystals with a photomultiplier tube (PMT) and a preamplifier.
 * 
 * In the present implementation, the case of the detector is divided into three parts:
 * 
 * - The front part, which contains the entrance window for radiation and encompasses most of the 
 * scintillator crystal.
 * - The PMT part, which contains the crystal-PMT interface and the PMT itself.
 * It is mounted inside a PMT case which acts as a magnetic shield.
 * - The connector part, which contains the connector.
 * 
 * The geometry is simplified in the sense that the front part of the detector, should resemble 
 * the real detector closely, while the back part consists of empty cases.
 * On the outside of the PMT case, two connectors for high voltage supply and signal readout are 
 * attached, which are implemented in a simplified way as well.
 */
class CeBr3_15x15 : public Detector{
    public:
        CeBr3_15x15(G4LogicalVolume *World_Logical, G4String name) : Detector (World_Logical, name){};

		void Construct(G4ThreeVector global_coordinates, G4double theta, G4double phi, G4double dist_from_center, G4double intrinsic_rotation_angle = 0.) override final;
        
};