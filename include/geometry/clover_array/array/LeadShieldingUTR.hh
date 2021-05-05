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
 * \brief Lead shielding in the UTR in the spring 2021 experiments.
 * 
 * The shielding consists of two brick walls and some lead sheets wrapped around the beam pipe, 
 * which are implemented in a simplistic way here.
 */
class LeadShieldingUTR{
public:
    LeadShieldingUTR(G4LogicalVolume* world_log):world_logical(world_log){};
    void Construct(const G4ThreeVector global_coordinates);

protected:
    G4LogicalVolume *world_logical;
};