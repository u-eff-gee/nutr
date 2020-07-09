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

#include "Scintillator_SCIONIX.hh"

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

	Scintillator_SCIONIX cebr1(world_logical, "cebr1");
	cebr1.Construct(G4ThreeVector(), 0., 0., 150.*mm);

	Scintillator_SCIONIX cebr2(world_logical, "cebr2");
	cebr2.Construct(G4ThreeVector(), 180.*deg, 0.*deg, 150.*mm);

	return world_phys;
}

void DetectorConstruction::ConstructSDandField(){

	SensitiveDetector *cebr1 = new SensitiveDetector("cebr1", "cebr1");
	cebr1->SetDetectorID(1);
	G4SDManager::GetSDMpointer()->AddNewDetector(cebr1);
	SetSensitiveDetector("cebr1", cebr1, true);

	SensitiveDetector *cebr2 = new SensitiveDetector("cebr2", "cebr2");
	cebr2->SetDetectorID(2);
	G4SDManager::GetSDMpointer()->AddNewDetector(cebr2);
	SetSensitiveDetector("cebr2", cebr2, true);
    
}
