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
 * \brief Lead shielding in the UTR during the first experiments of the
 * fall-2021 campaign.
 *
 * The shielding consists of three brick walls upstream of the Clover Array.
 * The most upstream brick wall is the one that is permanently installed.
 * In downstream direction, it is followed by the reinforced wall from the
 * spring-2021 campaign and another new wall that touches the outer edge of the
 * Clover Array mechanical structure.
 */
class LeadShieldingUTR {
public:
  LeadShieldingUTR(G4LogicalVolume *world_log) : world_logical(world_log){};
  void Construct(const G4ThreeVector global_coordinates);

protected:
  G4LogicalVolume *world_logical;
};