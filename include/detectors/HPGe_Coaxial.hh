/*
utr - Geant4 simulation of the UTR at HIGS
Copyright (C) 2017 the developing team (see README.md)

This file is part of utr.

utr is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

utr is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with utr.  If not, see <http://www.gnu.org/licenses/>.
*/

// Class for a generic coaxial High-purity Germanium (HPGe) detector

#pragma once

#include <vector>

#include "G4LogicalVolume.hh"

#include "Detector.hh"
#include "HPGe_Coaxial_Properties.hh"

using std::vector;

class HPGe_Coaxial : public Detector{
	public:
		HPGe_Coaxial(const G4String name, const HPGe_Coaxial_Properties prop, const G4double theta, const G4double phi, const G4double dist_from_center, const vector<Filter> filters = {}, const vector<Filter> wraps = {}, G4double intrinsic_rotation_angle = 0.) : Detector (name, theta, phi, dist_from_center, filters, wraps, intrinsic_rotation_angle), properties(prop), use_filter_case(false), use_filter_case_ring(false), use_dewar(false){};

		void Construct(G4LogicalVolume* world_logical, G4ThreeVector global_coordinates) override;
		void useFilterCase(){use_filter_case = true;};
		void useFilterCaseRing(){use_filter_case_ring = true;};
		void useDewar(){use_dewar = true;};

	private:
		const HPGe_Coaxial_Properties properties;
		bool use_filter_case;
		bool use_filter_case_ring;
		bool use_dewar;
};
