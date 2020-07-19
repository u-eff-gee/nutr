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

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

#include "DetectorConstruction.hh"

#include "Scintillator_SCIONIX.hh"

G4VPhysicalVolume* DetectorConstruction::Construct()
{
	G4NistManager *nist_manager = G4NistManager::Instance();

	world_solid = unique_ptr<G4Box>(new G4Box("world_solid", 2.*m, 2.*m, 2.*m));
	world_logical = unique_ptr<G4LogicalVolume>(new G4LogicalVolume(world_solid.get(), nist_manager->FindOrBuildMaterial("G4_AIR"), "world_logical"));
	world_logical->SetVisAttributes(G4VisAttributes::GetInvisible());
	world_phys = unique_ptr<G4VPhysicalVolume>(new G4PVPlacement(0, G4ThreeVector(), world_logical.get(), "world", 0, false, 0));

	Scintillator_SCIONIX cebr1(world_logical.get(), "cebr1");
	cebr1.Construct(G4ThreeVector(), 0., 0., 150.*mm);
    RegisterSensitiveLogicalVolumes(cebr1.get_sensitive_logical_volumes());

	Scintillator_SCIONIX cebr2(world_logical.get(), "cebr2");
	cebr2.Construct(G4ThreeVector(), 180.*deg, 0.*deg, 150.*mm);
    RegisterSensitiveLogicalVolumes(cebr2.get_sensitive_logical_volumes());

	return world_phys.get();
}