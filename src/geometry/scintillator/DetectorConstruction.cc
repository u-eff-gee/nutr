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

#include <memory>

using std::make_unique;

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

#include "DetectorConstruction.hh"

#include "CeBr3_15x15.hh"

G4VPhysicalVolume* DetectorConstruction::Construct()
{
	G4NistManager *nist_manager = G4NistManager::Instance();

	world_solid = make_unique<G4Box>("world_solid", 2.*m, 2.*m, 2.*m);
	world_logical = make_unique<G4LogicalVolume>(world_solid.get(), nist_manager->FindOrBuildMaterial("G4_AIR"), "world_logical");
	world_logical->SetVisAttributes(G4VisAttributes::GetInvisible());
	world_phys = make_unique<G4PVPlacement>(new G4RotationMatrix(), G4ThreeVector(), world_logical.get(), "world", nullptr, false, 0);

	CeBr3_15x15 cebr1(world_logical.get(), "cebr1");
	cebr1.Construct(G4ThreeVector(), 0., 0., 8.*25.4*mm);
    RegisterSensitiveLogicalVolumes(cebr1.get_sensitive_logical_volumes());

	CeBr3_15x15 cebr2(world_logical.get(), "cebr2");
	cebr2.Construct(G4ThreeVector(), 180.*deg, 0.*deg, 8.*25.4*mm);
    RegisterSensitiveLogicalVolumes(cebr2.get_sensitive_logical_volumes());

	return world_phys.get();
}
