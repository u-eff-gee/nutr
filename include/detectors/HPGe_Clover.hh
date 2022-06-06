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

#pragma once

#include "G4ExtrudedSolid.hh"
#include "G4LogicalVolume.hh"

#include "Detector.hh"
#include "HPGe_Clover_Properties.hh"

/**
 * \brief Clover-type high-purity germanium (HPGe) detector
 *
 * This class models a clover-type detector whose four crystals are made from
 * identical cylindric pieces of germanium. To be able to achieve a compact and
 * quadratic geometry, the cylindric crystals are flattened on four sides. The
 * geometry is mainly based on a "Germanium 2 Fold Segmented Clover Detector
 * OPERATING MANUAL" by Eurisys Mesures from 1997 that came with one of the
 * "Yale clover detectors" (V. Werner, private communication). The Eurisys
 * manual only shows the central holes in the germanium crystals for the anode
 * implicitly, but an experimental study of similar detectors [1] proves that
 * they exist and provided the basis for modeling the holes in this code. The
 * radius of the hole given in [1] (5 mm) agrees with the size of the implied
 * holes in the Eurisys manual. No value is given for the length of the hole,
 * but it is 8 mm shorter than the crystal length in [1].
 *
 * [1] S. A. A. Gros, "Characterization of an EXOGAM Clover", Phd thesis,
 * University of Liverpool (2005)
 */
class HPGe_Clover : public Detector {
public:
  HPGe_Clover(const string _name, const HPGe_Clover_Properties _prop,
              const double _theta, const double _phi,
              const double _dist_from_center,
              const FilterConfiguration _filter_configuration = {{}},
              const vector<Filter> _wraps = {},
              const double _intrinsic_rotation_angle = 0.,
              const double _dead_layer = 0.)
      : Detector(_name, _theta, _phi, _dist_from_center, _filter_configuration,
                 _wraps, _intrinsic_rotation_angle, _dead_layer,
                 _prop.end_cap_front_side_length),
        properties(_prop), use_dewar(true){};

  void Construct_Detector(G4LogicalVolume *world_logical,
                          G4ThreeVector global_coordinates) override final;
  void Construct_Filter_Case(G4LogicalVolume *world_logical,
                             G4ThreeVector global_coordinates,
                             double filter_dist_from_center) override final;
  G4VSolid *Filter_Shape(const string name,
                         const Filter &filter) const override final;
  void useDewar() { use_dewar = true; };

private:
  static G4VSolid *rounded_box(const string name, const double side_length,
                               const double length,
                               const double rounding_radius,
                               const int n_points_per_corner);
  const HPGe_Clover_Properties properties;
  bool use_dewar;
};
