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
#include "G4Color.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

#include "DetectorConstruction.hh"
#include "TrackerSD.hh"

DetectorConstruction::DetectorConstruction()
:G4VUserDetectorConstruction()
{}

DetectorConstruction::~DetectorConstruction()
{}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
	G4NistManager *nist_manager = G4NistManager::Instance();

	G4Box *world_solid = new G4Box("world_solid", 0.5*m, 0.5*m, 0.5*m);
	G4LogicalVolume *world_logical = new G4LogicalVolume(world_solid, nist_manager->FindOrBuildMaterial("G4_AIR"), "world_logical");
	G4VPhysicalVolume *world_phys = new G4PVPlacement(0, G4ThreeVector(), world_logical, "world", 0, false, 0);

	G4Box *water_solid = new G4Box("water_solid", 0.25*m, 0.25*m, 0.25*m);
	G4LogicalVolume *water_logical = new G4LogicalVolume(water_solid, nist_manager->FindOrBuildMaterial("G4_WATER"), "water_logical");
	water_logical->SetVisAttributes(new G4VisAttributes(G4Color::Blue()));
	new G4PVPlacement(0, G4ThreeVector(), water_logical, "water", world_logical, false, 0);

	return world_phys;
}

void DetectorConstruction::ConstructSDandField(){

	TrackerSD *water_tracker = new TrackerSD("water", "water");
	water_tracker->SetDetectorID(0);
	G4SDManager::GetSDMpointer()->AddNewDetector(water_tracker);
	SetSensitiveDetector("water_logical", water_tracker, true);
}
