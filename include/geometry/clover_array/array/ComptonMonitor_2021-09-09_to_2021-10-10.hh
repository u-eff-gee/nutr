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

    Copyright (C) 2020, 2021 Udo Friman-Gayer
*/

#pragma once

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

/**
 * \brief Lead housing of Compton beam monitor
 */
class ComptonMonitor {
public:
  ComptonMonitor(G4LogicalVolume *world_log) : world_logical(world_log){};
  void Construct(const G4ThreeVector global_coordinates);

  static constexpr double detector_angle = 40. * deg;

protected:
  G4LogicalVolume *world_logical;
};
