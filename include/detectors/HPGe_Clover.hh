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

// Class for a generic High-purity Germanium (HPGe) clover detector

#pragma once

#include "G4ExtrudedSolid.hh"
#include "G4LogicalVolume.hh"

#include "Detector.hh"
#include "HPGe_Clover_Properties.hh"

class HPGe_Clover : public Detector
{
public:
    HPGe_Clover(const G4String name, const HPGe_Clover_Properties prop, const G4double theta, const G4double phi, const G4double dist_from_center, const vector<Filter> filters = {}, const vector<Filter> wraps = {}, G4double intrinsic_rotation_angle = 0.)
    : Detector(name, theta, phi, dist_from_center, filters, wraps, intrinsic_rotation_angle), properties(prop), use_dewar(true) {};

    void Construct(G4LogicalVolume* world_logical, G4ThreeVector global_coordinates) override;
    void useDewar() { use_dewar = true; };
private:
    static G4VSolid* rounded_box(const G4String name, const G4double side_length, const G4double length, const G4double rounding_radius, const G4int n_points_per_corner);
    const HPGe_Clover_Properties properties;
    bool use_dewar;
};
