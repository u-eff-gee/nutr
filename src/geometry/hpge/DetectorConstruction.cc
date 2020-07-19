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

#include "HPGe_Coaxial.hh"
#include "HPGe_Collection.hh"

G4VPhysicalVolume* DetectorConstruction::Construct()
{
	G4NistManager* nist_manager = G4NistManager::Instance();

	world_solid = unique_ptr<G4Box>(new G4Box("world_solid", 2.*m, 2.*m, 2.*m));
	world_logical = unique_ptr<G4LogicalVolume>(new G4LogicalVolume(world_solid.get(), nist_manager->FindOrBuildMaterial("G4_AIR"), "world_logical"));
	world_logical->SetVisAttributes(G4VisAttributes::GetInvisible());
	world_phys = unique_ptr<G4VPhysicalVolume>(new G4PVPlacement(0, G4ThreeVector(), world_logical.get(), "world", 0, false, 0));

	HPGe_Collection hpge_collection;

	HPGe_Coaxial hpge1(world_logical.get(), "hpge1", HPGe_Collection::HPGe_60_TUNL_21033);
	hpge1.useDewar();
	hpge1.Construct(G4ThreeVector(), 90.*deg, 0.*deg, 100.*mm);
	RegisterSensitiveLogicalVolumes(hpge1.get_sensitive_logical_volumes());

	HPGe_Coaxial hpge2(world_logical.get(), "hpge2", hpge_collection.HPGe_60_TUNL_21033);
	hpge2.useDewar();
	hpge2.Construct(G4ThreeVector(), 90.*deg, 90.*deg, 100.*mm);
	RegisterSensitiveLogicalVolumes(hpge2.get_sensitive_logical_volumes());

	HPGe_Coaxial hpge3(world_logical.get(), "hpge3", hpge_collection.HPGe_60_TUNL_21033);
	hpge3.useDewar();
	hpge3.Construct(G4ThreeVector(), 90.*deg, 180.*deg, 100.*mm);
	RegisterSensitiveLogicalVolumes(hpge3.get_sensitive_logical_volumes());

	HPGe_Coaxial hpge4(world_logical.get(), "hpge4", hpge_collection.HPGe_60_TUNL_21033);
	hpge4.useDewar();
	hpge4.Construct(G4ThreeVector(), 90.*deg, 270.*deg, 100.*mm);
	RegisterSensitiveLogicalVolumes(hpge4.get_sensitive_logical_volumes());

	return world_phys.get();
}