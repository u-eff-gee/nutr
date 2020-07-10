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

    Copyright (C) 2020 Udo Friman-Gayer
*/

#pragma once

#include <vector>

using std::vector;

#include "G4LogicalVolume.hh"
#include "G4VUserDetectorConstruction.hh"

class G4VPhysicalVolume;

class NDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    NDetectorConstruction();
    virtual ~NDetectorConstruction();

    virtual G4VPhysicalVolume* Construct() = 0;
    void ConstructSDandField() override final;

    void RegisterSensitiveLogicalVolumes(vector<G4LogicalVolume*> logical_volumes);

protected:
    vector<G4LogicalVolume*> sensitive_logical_volumes;
};
