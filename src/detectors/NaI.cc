/*
utr - Geant4 simulation of the UTR at HIGS
Copyright (C) 2017 the developing team (see README.md)

This file is part of utr.

utr is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

utr is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with utr.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <algorithm>
#include <string>

using std::string;
using std::to_string;
using std::transform;

#include "G4Color.hh"
#include "G4NistManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4Polycone.hh"
#include "G4PVPlacement.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"

#include "NaI.hh"

void NaI::Construct(G4ThreeVector global_coordinates, G4double theta, G4double phi, G4double dist_from_center, G4double intrinsic_rotation_angle) {

	G4NistManager *nist = G4NistManager::Instance();
	G4ThreeVector e_r = unit_vector_r(theta, phi);
	G4ThreeVector e_theta = unit_vector_theta(theta, phi);

	rotate(theta, phi, intrinsic_rotation_angle);

	// Variables for the Mother Volume
	G4double NaIPlugMotherHLength = (297.94*0.5)*mm; //half-length of the cylinder
	G4double NaIPlugMotherRadius = ((162.050001*0.5))*mm;
	G4double NaIPlugMotherZpos = -NaIPlugMotherHLength;

	// Variables for the Al can 
	G4double naiplugcanThickness = 1.0*mm;
	//G4double naiplugcanOuterRad = NaIPlugMotherRadius-1.0*mm;
	//G4double naiplugcanInnerRad = naiplugcanOuterRad-naiplugcanThickness;
	//G4double naiplugcanHLen = 150.0*mm;

	G4double naiplugcanLength = NaIPlugMotherHLength*2.-(0.1*nm);
	G4double naiplugcanKink1 = 84.145*mm;
	G4double naiplugcanKink2 = 123.83*mm;
	G4double naiplugcanKink3a = 176.1*mm;
	G4double naiplugcanKink3b = 176.2*mm;
	G4double naiplugcanKink4a = 190.4*mm;
	G4double naiplugcanKink4b = 190.5*mm;
	//G4double naiplugcanbackOuterRad = (119.38/4.);
	//G4double naiplugcanbackInnerRad = naiplugcanbackOuterRad-naiplugcanThickness;

	G4double naiplugOuterRad1 = (61.6*0.5)*mm;
	G4double naiplugOuterRad2 = (61.6*0.5)*mm;
	G4double naiplugOuterRad3 = (61.6*0.5)*mm;
	G4double naiplugOuterRad4 = (140.97*0.5)*mm;
	G4double naiplugOuterRad5 = (140.97*0.5)*mm;
	G4double naiplugOuterRad6 = (162.05*0.5)*mm;
	G4double naiplugOuterRad7 = (162.05*0.5)*mm;
	G4double naiplugOuterRad8 = (158.65*0.5)*mm;
	G4double naiplugOuterRad9 = (158.65*0.5)*mm;
	G4double naiplugOuterRad10 = (158.65*0.5)*mm;

	G4double naiplugInnerRad1 = 0.*mm;//naiplugOuterRad1-naiplugcanThickness;
	G4double naiplugInnerRad2 = naiplugOuterRad2-naiplugcanThickness;
	G4double naiplugInnerRad3 = naiplugOuterRad3-naiplugcanThickness;
	G4double naiplugInnerRad4 = naiplugOuterRad4-naiplugcanThickness;
	G4double naiplugInnerRad5 = naiplugOuterRad5-naiplugcanThickness;
	G4double naiplugInnerRad6 = naiplugOuterRad6-naiplugcanThickness;
	G4double naiplugInnerRad7 = naiplugOuterRad7-naiplugcanThickness;
	G4double naiplugInnerRad8 = naiplugOuterRad8-naiplugcanThickness;
	G4double naiplugInnerRad9 = naiplugOuterRad9-naiplugcanThickness;
	G4double naiplugInnerRad10 = 0.;//naiplugOuterRad10-naiplugcanThickness;

	// variables for the NaI(Tl) detector
	//   G4double naiplugRad =  naiplugInnerRad8-1.0*mm;
		G4double naiplugRad = (152.4*0.5)*mm;
		G4double naiplugHLen = ((101.6)*0.5)*mm;
	
	//     G4double outerHousingRadius = (158.65*0.5)*mm;
	//     G4double halfHousingHeight = ((121.74-14.3)*0.5)*mm;

	///// CREATE A MOTHER VOLUME TO BUILD THE NaI(Tl) PLUG IN /////
	G4Tubs* NaIPlugMotherCylinder_solid
		= new G4Tubs("NaIPlug1_Mother",0,NaIPlugMotherRadius,NaIPlugMotherHLength,0,twopi);
			
	G4LogicalVolume* NaIPlugMotherCylinder_log =
		new G4LogicalVolume(NaIPlugMotherCylinder_solid, nist->FindOrBuildMaterial("G4_AIR"), "NaI Plug 1 Mother log",0,0,0);

	//Placing Mother cylinder	
	// The y values of the G4ThreeVector below are based on the dimensions of the shield structure 
	// (see section DetectorConstruction::BuildShield() -- around line 1000)		
	new G4PVPlacement(rotation_matrix,
				global_coordinates + (dist_from_center + NaIPlugMotherHLength)*e_r,
				NaIPlugMotherCylinder_log,
				"NaI Plug 1 Detector Mother Volume",
				world_Logical,
				false,
				0);
				
					
	////// BUILD THE Al CAN TO HOLD THE DETECTOR /////

	const G4int Nnaiplugz = 12;//number of sections 

	vector<G4double> naiplugcanrz{0*mm,naiplugcanThickness,naiplugcanThickness,naiplugcanKink1,naiplugcanKink2,naiplugcanKink3a,naiplugcanKink3b,naiplugcanKink4a,naiplugcanKink4b,naiplugcanLength-naiplugcanThickness,naiplugcanLength-naiplugcanThickness,naiplugcanLength};
	transform(naiplugcanrz.begin(), naiplugcanrz.end(), naiplugcanrz.begin(), [&NaIPlugMotherHLength](G4double z){ return 2.*NaIPlugMotherHLength-z; });
	vector<G4double> naiplugcanrinner{naiplugInnerRad1,naiplugInnerRad1,naiplugInnerRad2,naiplugInnerRad3,naiplugInnerRad4,naiplugInnerRad5,naiplugInnerRad6,naiplugInnerRad7,naiplugInnerRad8,naiplugInnerRad9,naiplugInnerRad10,naiplugInnerRad10};
	vector<G4double> naiplugcanrouter{naiplugOuterRad1,naiplugOuterRad1,naiplugOuterRad2,naiplugOuterRad3,naiplugOuterRad4,naiplugOuterRad5,naiplugOuterRad6,naiplugOuterRad7,naiplugOuterRad8,naiplugOuterRad9,naiplugOuterRad10,naiplugOuterRad10};

	G4Polycone *NaIPlugCan_solid
		= new G4Polycone("NaI Plug 1 Can",0,twopi,Nnaiplugz,naiplugcanrz.data(),naiplugcanrinner.data(),naiplugcanrouter.data());
		
	G4LogicalVolume *NaIPlugCan_log = 
		new G4LogicalVolume(NaIPlugCan_solid,nist->FindOrBuildMaterial("G4_Al"),"NaI Plug 1 Can log",0,0,0);

	// Back Housing
	new G4PVPlacement(0,
				G4ThreeVector(0.*mm,0.*cm,NaIPlugMotherZpos),
				NaIPlugCan_log,
				"NaI Plug 1 Can",
				NaIPlugMotherCylinder_log,
				false,
				0);

	///// BUILD AND PLACE THE NaI(Tl) DETECTOR /////

	G4Tubs* RefPlug_solid 
		= new G4Tubs("Ref Material", 0.*mm, naiplugRad, naiplugHLen, 0,twopi);
		
	G4LogicalVolume* RefPlug1_log = 
		new G4LogicalVolume(RefPlug_solid,nist->FindOrBuildMaterial("G4_MAGNESIUM_OXIDE"),"Reflective material log",0,0,0);

	// Crystal Housing
	new G4PVPlacement(0,
				G4ThreeVector(0.*mm,0.*cm,-(NaIPlugMotherHLength-naiplugHLen-naiplugcanThickness-1.*mm-1.5*mm)),//1mm for thickness of can, then 1.5mm back from front edge of NaI plug can
				RefPlug1_log,
				"NaI Plug 1",
				NaIPlugMotherCylinder_log,
				false,
				1);

	G4Tubs* NaIPlug_solid 
		= new G4Tubs("NaI Plug 1", 0.*mm, naiplugRad-0.25, naiplugHLen-0.25, 0,twopi);

	// Crystal material
	G4Material* NaITl = new G4Material("NaI(Tl)_"+detector_name, 3.67*g/cm3,2);
	NaITl->AddMaterial(nist->FindOrBuildMaterial("G4_SODIUM_IODIDE"), 99.*perCent);
	NaITl->AddElement(nist->FindOrBuildElement("Tl"), 1.*perCent);

	sensitive_logical_volumes.push_back(new G4LogicalVolume(NaIPlug_solid,NaITl,detector_name,0,0,0));

	//Crystal
	new G4PVPlacement(0,
				G4ThreeVector(0.*mm,(0.)*cm,0.*mm),//1mm for thickness of can, then 1.5mm back from front edge of NaI plug can
				sensitive_logical_volumes[0],
				"NaI Plug 1",
				RefPlug1_log,
				false,
				0);
	
	NaIPlugMotherCylinder_log->SetVisAttributes(G4VisAttributes::Invisible);

	//   G4VisAttributes* motherVisAtt = new G4VisAttributes(G4Colour(0.,0.,1));
	//   naiplugcanVisAtt->SetForceWireframe(true);
	//   motherVisAtt->SetForceSolid(true);
	//   NaIPlugMotherCylinder_log->SetVisAttributes(motherVisAtt);

	G4VisAttributes* naiplugcanVisAtt = new G4VisAttributes(G4Colour(0.8,0.8,0.8));
	//   naiplugcanVisAtt->SetForceWireframe(true);
	// naiplugcanVisAtt->SetForceSolid(true);
	NaIPlugCan_log->SetVisAttributes(naiplugcanVisAtt);

	G4VisAttributes* refPlug1Att = new G4VisAttributes(G4Colour());
	refPlug1Att->SetForceWireframe(true);
	//   refPlug1Att->SetForceSolid(true);
	RefPlug1_log->SetVisAttributes(refPlug1Att);

	G4VisAttributes* naiplugVisAtt = new G4VisAttributes(G4Colour(0.,0.75,0.));
	sensitive_logical_volumes[0]->SetVisAttributes(naiplugVisAtt);
}