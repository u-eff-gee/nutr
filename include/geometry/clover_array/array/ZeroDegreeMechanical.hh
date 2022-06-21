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

#pragma once

#include "G4LogicalVolume.hh"

/**
 * \brief Zero-degree detector mechanical setup
 *
 * At the moment, this class implements the NaI annulus around the zero-degree
 * detector. Although the annulus is a detector by itself, it has never been
 * used during the author's career. Therefore, it is simply implemented as a
 * passive component here.
 */
class ZeroDegreeMechanical {
public:
  ZeroDegreeMechanical(G4LogicalVolume *world_log) : world_logical(world_log){};
  void Construct(const G4ThreeVector global_coordinates);

  static constexpr double zero_degree_to_target =
      108.8 * 25.4 * mm; // 108.8 inch
  static constexpr double offset_y = 30. * mm;

protected:
  G4LogicalVolume *world_logical;
};