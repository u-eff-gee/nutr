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

class HPGe_Coaxial : public Detector {
public:
  HPGe_Coaxial(const G4String _name, const HPGe_Coaxial_Properties _prop,
               const G4double _theta, const G4double _phi,
               const G4double _dist_from_center,
               const vector<Filter> _filters = {},
               const vector<Filter> _wraps = {},
               G4double _intrinsic_rotation_angle = 0.)
      : Detector(_name, _theta, _phi, _dist_from_center, _filters, _wraps,
                 _intrinsic_rotation_angle, _prop.end_cap_outer_radius),
        properties(_prop), use_dewar(true){};

  void Construct_Detector(G4LogicalVolume *world_logical,
                          G4ThreeVector global_coordinates) override final;
  G4VSolid *Filter_Shape(const string name,
                         const Filter &filter) const override final;
  void useDewar() { use_dewar = true; };

private:
  const HPGe_Coaxial_Properties properties;
  bool use_dewar;
};
