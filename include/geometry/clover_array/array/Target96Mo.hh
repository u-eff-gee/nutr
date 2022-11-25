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

    Copyright (C) 2021 Oliver Papst
*/

#pragma once

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "SourceVolumeTubs.hh"

/**
 * \brief 96Mo target provided by R. Schwengner, HZDR
 */
class Target96Mo {
public:
  Target96Mo(G4LogicalVolume *world_log) : world_logical(world_log){};
  void Construct(const G4ThreeVector global_coordinates);

  static constexpr double diameter_96mo = 2. * cm;
  static constexpr double side_pe = 2.8 * cm;
  static constexpr double thickness_tot = 0.111 * cm;
  static constexpr double mass_96mo = 1949.0 * mg;
  static constexpr double mass_pe = 50.8 * mg;
  static constexpr double density_pe = 0.940 * mg / (mm * mm * mm);

  std::shared_ptr<SourceVolume> get_source_volume() {
    return source_volume;
  }

protected:
  G4LogicalVolume *world_logical;
  std::shared_ptr<SourceVolume> source_volume;
};
