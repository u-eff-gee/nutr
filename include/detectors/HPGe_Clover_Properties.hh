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

// Struct that contains the properties of a specific
// clover-type High-purity Germanium (HPGe) detector

#pragma once

#include <string_view>

using std::string_view;

struct HPGe_Clover_Properties {
  // Struct that contains the dimensions of a clover detector
  //
  // 'Front' = towards the target
  // 'Back' = away from the target

  // Detector crystals
  double crystal_radius; // Radius of the detector crystals if they were still
                         // round
  double crystal_length; // Length of the detector crystal
  double crystal_face_radius; // Rounding radius of the crystal face
  // The contact layers on the surface of the crystal are neglected at the
  // moment, since they usually have a thickness of less than a millimeter and
  // are made of very light materials
  double crystal_gap; // The four crystals do not touch each other, but there
                      // is some gap in between
  double end_cap_to_crystal_gap_front; // Vacuum-filled gap between the end
                                       // cap and the detector
                                       // crystals at the front
  double vacuum_length; // Length of the evacuated area inside the end cap
  double
      anode_length; // Length of the anode hole from the back of each crystal.
  double anode_radius; // Radius of the anode hole.

  // End cap
  // Case with quadratic base area and smoothed edges which covers the whole
  // mount cup. It consists of the fron part that contains the detector crystals
  // and a back part that contains the electronics. The back part is modelled as
  // an skeleton, since the inner composition is unknown.
  double end_cap_front_side_length; // Side length of the front end cap with a
                                    // quadratic (see next variable) base area
  double end_cap_front_rounding_radius; // Rounding radius of the edges
  double end_cap_front_length;     // Length of the end cap around the crystal.
  double end_cap_front_thickness;  // Thickness of the end cap at the front
                                   // (But not at the part that faces the
                                   // target. This 'window thickness' (see
                                   // below) may be thinner.)
  double end_cap_window_thickness; // Thickness of the window which faces the
                                   // target

  /* The following parts are implemented mainly for aesthetic reasons and
   * they are rough estimates for the actual structure of the end cap at
   * the back, the dewar and the connecting piece.
   */

  double end_cap_back_side_length; // Side length of the back end cap with a
                                   // quadratic (see next variable) base area
  double end_cap_back_rounding_radius; // Rounding radius of the edges
  double end_cap_back_length; // Length of the end cap around the electronics.
  double end_cap_back_thickness; // Thickness of the end cap
  string_view end_cap_material;

  // Connection between dewar and mount cup / end cap
  // Contains the cold finger and electronics, but the latter are not
  // implemented here. Assumed to consist of solid aluminium.
  double connection_length; // Length of the connecting piece
  double connection_radius; // Radius of the connecting piece
  string_view connection_material;

  // Dewar
  // A cylindric tube with top and bottom
  double dewar_length;         // Dewar length, including top and bottom
  double dewar_outer_radius;   // Outer radius, can be measured most easily
  double dewar_wall_thickness; // Dewar wall thickness. Assumed to be the same
                               // for top, bottom and side
  string_view dewar_material;
};
