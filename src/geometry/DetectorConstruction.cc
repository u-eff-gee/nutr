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
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

#include "DetectorConstruction.hh"
#include "SensitiveDetector.hh"

#include "HPGe_Coaxial.hh"
#include "HPGe_Collection.hh"

DetectorConstruction::DetectorConstruction()
:G4VUserDetectorConstruction()
{}

DetectorConstruction::~DetectorConstruction()
{}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
	G4NistManager *nist_manager = G4NistManager::Instance();

	G4Box *world_solid = new G4Box("world_solid", 2.*m, 2.*m, 2.*m);
	G4LogicalVolume *world_logical = new G4LogicalVolume(world_solid, nist_manager->FindOrBuildMaterial("G4_AIR"), "world_logical");
	world_logical->SetVisAttributes(G4VisAttributes::GetInvisible());
	G4VPhysicalVolume *world_phys = new G4PVPlacement(0, G4ThreeVector(), world_logical, "world", 0, false, 0);

	HPGe_Collection hpge_collection;

	HPGe_Coaxial hpge1(world_logical, "hpge1");
	hpge1.setProperties(hpge_collection.HPGe_60_TUNL_21033);
	hpge1.useDewar();
	hpge1.Construct(G4ThreeVector(), 90.*deg, 0.*deg, 100.*mm);

	HPGe_Coaxial hpge2(world_logical, "hpge2");
	hpge2.setProperties(hpge_collection.HPGe_60_TUNL_21033);
	hpge2.useDewar();
	hpge2.Construct(G4ThreeVector(), 90.*deg, 90.*deg, 100.*mm);

	HPGe_Coaxial hpge3(world_logical, "hpge3");
	hpge3.setProperties(hpge_collection.HPGe_60_TUNL_21033);
	hpge3.useDewar();
	hpge3.Construct(G4ThreeVector(), 90.*deg, 180.*deg, 100.*mm);

	HPGe_Coaxial hpge4(world_logical, "hpge4");
	hpge4.setProperties(hpge_collection.HPGe_60_TUNL_21033);
	hpge4.useDewar();
	hpge4.Construct(G4ThreeVector(), 90.*deg, 270.*deg, 100.*mm);

	return world_phys;
}

void DetectorConstruction::ConstructSDandField(){

	SensitiveDetector *hpge1 = new SensitiveDetector("hpge1", "hpge1");
	hpge1->SetDetectorID(1);
	G4SDManager::GetSDMpointer()->AddNewDetector(hpge1);
	SetSensitiveDetector("hpge1", hpge1, true);

	SensitiveDetector *hpge2 = new SensitiveDetector("hpge2", "hpge2");
	hpge2->SetDetectorID(2);
	G4SDManager::GetSDMpointer()->AddNewDetector(hpge2);
	SetSensitiveDetector("hpge2", hpge2, true);

	SensitiveDetector *hpge3 = new SensitiveDetector("hpge3", "hpge3");
	hpge3->SetDetectorID(3);
	G4SDManager::GetSDMpointer()->AddNewDetector(hpge3);
	SetSensitiveDetector("hpge3", hpge3, true);

	SensitiveDetector *hpge4 = new SensitiveDetector("hpge4", "hpge4");
	hpge4->SetDetectorID(4);
	G4SDManager::GetSDMpointer()->AddNewDetector(hpge4);
	SetSensitiveDetector("hpge4", hpge4, true);
}
