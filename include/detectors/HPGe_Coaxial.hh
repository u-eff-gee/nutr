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

    Copyright (C) 2020-2022 Udo Friman-Gayer and Oliver Papst
*/

/**
 * This file contains code from the "utr" Geant4 simulation project.
 *
 * U. Friman-Gayer, J. Kleemann, and O. Papst, GEANT4 simulation of the Upstream
 * Target ROOM (UTR) at the HIγS facility (v2019.09), Zenodo (2019)
 * https://doi.org/10.5281/zenodo.3430154
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
  HPGe_Coaxial(const string _name, const HPGe_Coaxial_Properties _properties,
               const HPGe_Coaxial_Dewar_Properties _dewar_properties,
               const double _theta, const double _phi,
               const double _dist_from_center,
               const FilterConfiguration _filters_configuration = {{}},
               const vector<Filter> _wraps = {},
               const double _intrinsic_rotation_angle = 0.,
               const vector<double> _dead_layer = {0.})
      : Detector(_name, _theta, _phi, _dist_from_center, _filters_configuration,
                 _wraps, _intrinsic_rotation_angle, _dead_layer,
                 _properties.end_cap_outer_radius),
        properties(_properties), dewar_properties(_dewar_properties){};

  void Construct_Detector(G4LogicalVolume *world_logical,
                          G4ThreeVector global_coordinates) override final;
  void Construct_Filter_Case(G4LogicalVolume *world_logical,
                             G4ThreeVector global_coordinates,
                             double filter_dist_from_center) override final;
  G4VSolid *Filter_Shape(const string name,
                         const Filter &filter) const override final;

private:
  const HPGe_Coaxial_Properties properties;
  const HPGe_Coaxial_Dewar_Properties dewar_properties;
};
