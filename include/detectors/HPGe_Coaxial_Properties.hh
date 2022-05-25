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

// This file implements two structs.
// 1) A struct 'HPGe_Coaxial_Properties' that contains the properties of the
// front part of a specific coaxial High-purity Germanium (HPGe) detector.
// 2) A struct 'HPGe_Coaxial_Dewar_Properties' that can be used to supply the
// properties of a standard axially symmetry liquid-nitrogen dewar. The dewar
// parts are implemented mainly for visualization reasons and they are rough
// estimates of the actual structure of the dewar and the connecting piece.
#pragma once

struct HPGe_Coaxial_Properties {
  // Struct that contains the dimensions of a coaxial detector
  // The naming is partially inspired by typical Ortec data sheets
  //
  // 'Face' = towards the target
  // 'Base' = away from the target

  // Detector crystal
  double detector_radius;      // Radius of the detector crystal
  double detector_length;      // Length of the detector crystal, including the
                               // rounded part
  double detector_face_radius; // Rounding radius of the crystal face
  double hole_radius; // Radius of the hole in the crystal which is penetrated
                      // by the cold finger
  double hole_depth;  // Depth of the hole, measured from the base of the
                      // crystal, including the rounded part
  double hole_face_radius; // Rounding radius of the tip of the hole. Most
                           // probably the same as the hole radius
  // The contact layers on the surface of the crystal are neglected at the
  // moment, since they usually have a thickness of less than a millimeter and
  // are made of very light materials

  // Mount cup
  // Cylindrical tube with top and bottom inside which the crystal is mounted.
  // (Top == Face in this case)
  // It is separated from the outer cup by a layer of vacuum.
  double mount_cup_length;    // Including top and bottom
  double mount_cup_thickness; // Thickness of the mount cup wall and top
  double
      mount_cup_base_thickness; // Thickness of the mount cup base, which may be
                                // different from the thickness of top and side
  string mount_cup_material;

  // End cap
  // Cylindrical cube which covers the whole mount cup.
  // The end cap length is assumed to be the same as the mount cup length plus
  // the vacuum layer
  double end_cap_to_crystal_gap_front; // Distance from the front of the mount
                                       // cup to the inside of the end cap
                                       // window, i.e. thickness of the vacuum
                                       // layer between both
  double
      end_cap_to_crystal_gap_side; // Distance from the side of the mount cup
                                   // to the inner side of the end cap, i.e.
                                   // thickness of the vacuum layer between both
  double end_cap_thickness;        // Thickness of the end cap side
  double end_cap_length; // Length of the end cap, including end cap window
                         // thickness
  double end_cap_outer_radius; // Outer radius of the end cap. Not used in the
                               // actual construction, but can be used for
                               // placements or calculation of the size of the
                               // vacuum gap
  double end_cap_window_thickness; // Thickness of the window which faces the
                                   // target
  string end_cap_material;
  string end_cap_window_material;

  // Cold finger
  // Conducts heat from the dewar to the detector crystal. It penetrates the
  // hole in the crystal and is attached to the connection. Therefore, its total
  // length depends on other dimensions.
  double cold_finger_radius; // Radius of the cold finger, which is equal to
                             // the rounding radius
  double cold_finger_penetration_depth; // Determines how far the cold finger,
                                        // including the rounded tip, reaches
                                        // into the crystal
  string cold_finger_material;
};

struct HPGe_Coaxial_Dewar_Properties {
  /* Struct that contains the dimensions of a coaxial detector's dewar and the
   * connecting piece between the front part and the dewar.
   */

  // Connection between dewar and mount cup / end cap
  // Contains the cold finger and electronics, but the latter are not
  // implemented here. Assumed to consist of solid aluminium.
  double connection_length = 0.; // Length of the connecting piece
  double connection_radius = 0.; // Radius of the connecting piece
  double dewar_offset =
      0.; // Offset of the symmetry axis of dewar and connection
          // from the symmetry axis of the front part.
  string connection_material = "";

  // Dewar
  // A cylindric tube with top and bottom
  double dewar_length = 0.;         // Dewar length, including top and bottom
  double dewar_outer_radius = 0.;   // Outer radius, can be measured most easily
  double dewar_wall_thickness = 0.; // Dewar wall thickness. Assumed to be the
                                    // same for top, bottom and side
  string dewar_material = "";
};
