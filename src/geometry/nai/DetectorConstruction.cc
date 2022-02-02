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
#include "G4Colour.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4PVPlacement.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

#include "DetectorConstruction.hh"
#include "NaI.hh"

const double inch = 25.4*mm;

G4VPhysicalVolume* DetectorConstruction::Construct()
{
	G4NistManager* nist_manager = G4NistManager::Instance();

	world_solid = make_unique<G4Box>("world_solid", 0.5*m, 0.5*m, 0.5*m);
	world_logical = make_unique<G4LogicalVolume>(world_solid.get(), nist_manager->FindOrBuildMaterial("G4_AIR"), "world_logical");
	world_logical->SetVisAttributes(G4VisAttributes::GetInvisible());
	world_phys = make_unique<G4PVPlacement>(new G4RotationMatrix(), G4ThreeVector(), world_logical.get(), "world", nullptr, false, 0);

	/********* NaI(Tl) Detectors *********/

	NaI nai1(world_logical.get(), "nai1");
	nai1.Construct(G4ThreeVector(), 0., 0., 18.5*mm);
	RegisterSensitiveLogicalVolumes(nai1.get_sensitive_logical_volumes());

	NaI nai2(world_logical.get(), "nai2");
	nai2.Construct(G4ThreeVector(), pi, pi, 18.5*mm);
	RegisterSensitiveLogicalVolumes(nai2.get_sensitive_logical_volumes());

	/********* Divider Wall *********/

	G4double DividerWallLengthX = 19.05*cm;
	G4double DividerWallLengthY = 16.05*cm;
	G4double DividerWallLengthZ = 2.54*cm;

	G4Box* DividerWall_solid = new G4Box("DividerWall", DividerWallLengthX*0.5, DividerWallLengthY*0.5, DividerWallLengthZ*0.5);

	G4Tubs* Hole_solid = new G4Tubs("Hole", 0., 1.9*cm, 0.5*inch, 0., twopi);

	const G4double bevel_cone_rmax = 26.67*mm;
	const G4double bevel_cone_height = bevel_cone_rmax;
	G4Cons* Bevel1_solid = new G4Cons ("Bevel1", 0., 0., 0., bevel_cone_rmax, 0.5*bevel_cone_height, 0., twopi); 
	G4Cons* Bevel2_solid = new G4Cons ("Bevel2", 0., bevel_cone_rmax, 0., 0., 0.5*bevel_cone_height, 0., twopi); 

	G4SubtractionSolid* DetectorDivider1_solid = new G4SubtractionSolid("DetectorDivider1", DividerWall_solid, Hole_solid);
	G4SubtractionSolid* DetectorDivider2_solid = new G4SubtractionSolid("DetectorDivider2", DetectorDivider1_solid, Bevel1_solid);
	G4SubtractionSolid* DetectorDivider	= new G4SubtractionSolid("DetectorDivider", DetectorDivider2_solid, Bevel2_solid);

	G4LogicalVolume* DetectorDivider_log = new G4LogicalVolume(DetectorDivider, nist_manager->FindOrBuildMaterial("G4_Pb"), "DividerWall Solid");
	DetectorDivider_log->SetVisAttributes(G4Colour::Grey());

	new G4PVPlacement(0, G4ThreeVector(), DetectorDivider_log, "DetectorDivider", world_logical.get(), false, 0);

	/********* Sample holder *********/

	G4Tubs* AluminumCylinder_solid = new G4Tubs("Aluminum Cylinder", 0., 1.85*cm, 1.35*cm, 0., twopi);  

	const G4double sample_holder_cavity_radius = 1.75*cm;
	const G4double sample_holder_cavity_length = 2.5*cm;
	G4Tubs* Cavity_solid = new G4Tubs("Cavity", 0., sample_holder_cavity_radius, 0.5*sample_holder_cavity_length, 0., twopi); 

	G4SubtractionSolid* SampleHolder_solid = new G4SubtractionSolid("Sample Holder", AluminumCylinder_solid, Cavity_solid); 

	G4LogicalVolume* SampleHolder_log = new G4LogicalVolume(SampleHolder_solid, nist_manager->FindOrBuildMaterial("G4_Al"), "Sample Holder");
	SampleHolder_log->SetVisAttributes(G4Colour::Cyan());

	new G4PVPlacement (0, G4ThreeVector(), SampleHolder_log, "Sample Holder", world_logical.get(), false, 0);

	// /********* Sample *********/

	// Sample material (Monazite Powder)
	G4Material* sample_material = new G4Material("Monazite", 0.551*g/cm3, 10); 
	sample_material->AddElement(nist_manager->FindOrBuildElement("Ce"),0.268); 
	sample_material->AddElement(nist_manager->FindOrBuildElement("La"),0.119);
	sample_material->AddElement(nist_manager->FindOrBuildElement("Pr"),0.024); 
	sample_material->AddElement(nist_manager->FindOrBuildElement("Nd"),0.078); 
	sample_material->AddElement(nist_manager->FindOrBuildElement("Th"),0.104);
	sample_material->AddElement(nist_manager->FindOrBuildElement("Y"),0.005); 
	sample_material->AddElement(nist_manager->FindOrBuildElement("U"),0.004); 
	sample_material->AddElement(nist_manager->FindOrBuildElement("Si"),0.002);
	sample_material->AddElement(nist_manager->FindOrBuildElement("P"),0.128);
	sample_material->AddElement(nist_manager->FindOrBuildElement("O"),0.268); 

	G4Tubs* Sample_solid = new G4Tubs("Sample", 0., sample_holder_cavity_radius, 0.5*sample_holder_cavity_length, 0., twopi);         

	G4LogicalVolume* Sample_log = new G4LogicalVolume(Sample_solid, sample_material, "Sample Solid");
	Sample_log->SetVisAttributes(G4Colour::Red());

	new G4PVPlacement(0, G4ThreeVector(), Sample_log, "Sample", world_logical.get(), false, 0); 

	return world_phys.get();
}