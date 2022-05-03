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

#include <string>

using std::string;

#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4VPhysicalVolume.hh"

/**
 * \brief Collimator room upstream of the UTR.
 *
 * The collimator room contains a collimator block of variable size and
 * material, and two lead walls. The dimensions were mostly taken from the
 * previous utr simulation.
 */
class CollimatorRoom {
public:
  CollimatorRoom(G4LogicalVolume *world_log,
                 const double col_rad = 0.75 * 25.4 * mm,
                 const double col_len = 300. * mm,
                 const string col_mat = "G4_Pb")
      : world_logical(world_log), collimator_radius(col_rad),
        collimator_length(col_len), collimator_material(col_mat){};
  void Construct(const G4ThreeVector global_coordinates);

protected:
  G4LogicalVolume *world_logical;
  const double collimator_radius;
  const double collimator_length;
  const string collimator_material;
};