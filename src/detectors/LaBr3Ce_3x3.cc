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

#include <sstream>

using std::stringstream;

#include "G4Color.hh"
#include "G4Cons.hh"
#include "G4NistManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

#include "LaBr3Ce.hh"
#include "LaBr3Ce_3x3.hh"

LaBr3Ce labr3ce;

void LaBr3Ce_3x3::Construct(G4ThreeVector global_coordinates, G4double theta, G4double phi, G4double dist_from_center, [[maybe_unused]] G4double intrinsic_rotation_angle) {

	G4NistManager *nist = G4NistManager::Instance();

	G4ThreeVector symmetry_axis(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta)); // Symmetry axis along which the single elements of the detector are constructed

	G4RotationMatrix* rotation = new G4RotationMatrix();
	rotation->rotateZ(-phi);
	rotation->rotateY(-theta);

	const G4double inch = 25.4*mm;
	const G4double crystal_length = 3.*inch; // BI
	const G4double crystal_radius = 3.*inch * 0.5; // BI
	const G4double crystal_housing_thickness = 0.5*mm; // BI
	const G4double crystal_housing_outer_radius = 3.25*inch*0.5; // BI
	const G4double vacuum_thickness_front = 2.*mm; // BI
	const G4double vacuum_thickness_back = 2.*mm; //BI

	const G4double circuit_housing_thickness = 2.*mm; // Estimated, determines the thickness of 
						// all circuit_housing elements
	const G4double circuit_housing_1_length = 2.25*inch; // Measured
	const G4double circuit_housing_1_radius = 3.5*inch *0.5; // Measured

	const G4double circuit_housing_2_length = 3.*cm; // Measured
	const G4double circuit_housing_2_rmin = 2.5*inch * 0.5; // Measured
	const G4double circuit_housing_2_rmax = 3.5*inch * 0.5; // Measured

	
	const G4double circuit_housing_3_length = 3.*cm; // Measured
	const G4double circuit_housing_3_radius = 2.5*inch *0.5; // Measured

	const G4double pmt_housing_length = 7.*cm; // Measured
	const G4double pmt_housing_radius = 2.5*inch * 0.5; // Measured

	/************** Crystal housing *************/

	G4Tubs *crystal_housing_face_solid = new G4Tubs(detector_name + "_crystal_housing_face_solid",
			0., crystal_housing_outer_radius, crystal_housing_thickness*0.5, 0., twopi);
	G4LogicalVolume *crystal_housing_face_logical = new G4LogicalVolume(crystal_housing_face_solid, nist->FindOrBuildMaterial("G4_Al"), detector_name + "_crystal_housing_face_logical");
	crystal_housing_face_logical->SetVisAttributes(new G4VisAttributes(G4Color::Grey()));

	new G4PVPlacement(rotation, global_coordinates + (dist_from_center + 0.5*crystal_housing_thickness)*symmetry_axis, crystal_housing_face_logical, detector_name + "_crystal_housing_face", world_Logical, 0, 0, false);

	G4double crystal_housing_case_length = vacuum_thickness_back + vacuum_thickness_front + crystal_length + 2.*crystal_housing_thickness;
	G4Tubs *crystal_housing_case_solid = new G4Tubs(detector_name + "_crystal_housing_case_solid",
			crystal_housing_outer_radius - crystal_housing_thickness, crystal_housing_outer_radius, crystal_housing_case_length*0.5, 0., twopi);
	G4LogicalVolume *crystal_housing_case_logical = new G4LogicalVolume(crystal_housing_case_solid, nist->FindOrBuildMaterial("G4_Al"), detector_name + "_crystal_housing_case_logical");
	crystal_housing_case_logical->SetVisAttributes(new G4VisAttributes(G4Color::Grey()));

	new G4PVPlacement(rotation, global_coordinates + (dist_from_center + crystal_housing_thickness + crystal_housing_case_length*0.5)*symmetry_axis, crystal_housing_case_logical, detector_name + "_crystal_housing_case", world_Logical, 0, 0, false);

	/************** Vacuum around crystal *************/

	G4Tubs *vacuum_solid = new G4Tubs(detector_name + "_vacuum_solid", 0.,
			crystal_housing_outer_radius - crystal_housing_thickness, crystal_housing_case_length*0.5, 0., twopi);
	G4LogicalVolume *vacuum_logical = new G4LogicalVolume(vacuum_solid, nist->FindOrBuildMaterial("G4_Galactic"), detector_name + "_vacuum_logical");
	vacuum_logical->SetVisAttributes(new G4VisAttributes(G4Color::Cyan()));

	new G4PVPlacement(rotation, global_coordinates + (dist_from_center + crystal_housing_thickness + crystal_housing_case_length*0.5)*symmetry_axis, vacuum_logical, detector_name + "_vacuum", world_Logical, 0, 0, false);

	/************** Detector crystal *************/

	G4Tubs *crystal_solid = new G4Tubs(detector_name + "_crystal_solid",
			0., crystal_radius, crystal_length*0.5, 0., twopi);
	sensitive_logical_volumes.push_back(new G4LogicalVolume(crystal_solid, G4Material::GetMaterial("LaBr3Ce"), detector_name));
	sensitive_logical_volumes[0]->SetVisAttributes(new G4VisAttributes(G4Color::Green()));

	new G4PVPlacement(0, G4ThreeVector(0., 0., vacuum_thickness_front + 0.5*(crystal_length -crystal_housing_case_length)), sensitive_logical_volumes[0], detector_name + "_crystal", vacuum_logical, 0, 0, false);

	/************** Circuit housing 1 *************/
	
	G4Tubs *circuit_housing_1_solid = new G4Tubs(detector_name + "_circuit_housing_1_solid",
			crystal_housing_outer_radius, circuit_housing_1_radius, circuit_housing_1_length*0.5, 0., twopi);
	G4LogicalVolume *circuit_housing_1_logical = new G4LogicalVolume(circuit_housing_1_solid, nist->FindOrBuildMaterial("G4_Al"), detector_name + "_circuit_housing_1_logical");
	circuit_housing_1_logical->SetVisAttributes(new G4VisAttributes(G4Color::Grey()));

	new G4PVPlacement(rotation, global_coordinates + (dist_from_center + crystal_housing_thickness + crystal_housing_case_length + circuit_housing_1_length*0.5)*symmetry_axis, circuit_housing_1_logical, detector_name + "_circuit_housing_1", world_Logical, 0, 0, false);

	/************** Circuit housing 2 *************/

	G4Cons *circuit_housing_2_solid =
		new G4Cons("circuit_housing_2_solid", circuit_housing_2_rmax - circuit_housing_thickness, circuit_housing_2_rmax, circuit_housing_2_rmin - circuit_housing_thickness, circuit_housing_2_rmin, circuit_housing_2_length*0.5, 0., twopi);
	G4LogicalVolume *circuit_housing_2_logical = new G4LogicalVolume(circuit_housing_2_solid, nist->FindOrBuildMaterial("G4_Al"), detector_name + "_circuit_housing_2_logical");
	circuit_housing_2_logical->SetVisAttributes(new G4VisAttributes(G4Color::Grey()));

	new G4PVPlacement(rotation, global_coordinates + (dist_from_center + crystal_housing_thickness + crystal_housing_case_length + circuit_housing_1_length + circuit_housing_2_length*0.5)*symmetry_axis, circuit_housing_2_logical, detector_name + "_circuit_housing_2", world_Logical, 0, 0, false);

	/************** Circuit housing 3 *************/
	
	G4Tubs *circuit_housing_3_solid = new G4Tubs(detector_name + "_circuit_housing_3_solid",
			circuit_housing_3_radius - circuit_housing_thickness, circuit_housing_3_radius, circuit_housing_3_length*0.5, 0., twopi);
	G4LogicalVolume *circuit_housing_3_logical = new G4LogicalVolume(circuit_housing_3_solid, nist->FindOrBuildMaterial("G4_Al"), detector_name + "_circuit_housing_3_logical");
	circuit_housing_3_logical->SetVisAttributes(new G4VisAttributes(G4Color::Grey()));

	new G4PVPlacement(rotation, global_coordinates + (dist_from_center + crystal_housing_thickness + crystal_housing_case_length  + circuit_housing_1_length + circuit_housing_2_length + circuit_housing_3_length*0.5)*symmetry_axis, circuit_housing_3_logical, detector_name + "_circuit_housing_3", world_Logical, 0, 0, false);

	/************** Circuit housing 4 *************/
	
	G4Tubs *pmt_housing_solid = new G4Tubs(detector_name + "_pmt_housing_solid",
			pmt_housing_radius - circuit_housing_thickness, pmt_housing_radius, pmt_housing_length*0.5, 0., twopi);
	G4LogicalVolume *pmt_housing_logical = new G4LogicalVolume(pmt_housing_solid, nist->FindOrBuildMaterial("G4_Al"), detector_name + "_pmt_housing_logical");
	pmt_housing_logical->SetVisAttributes(new G4VisAttributes(G4Color::Grey()));

	new G4PVPlacement(rotation, global_coordinates + (dist_from_center + crystal_housing_thickness + crystal_housing_case_length  + circuit_housing_1_length + circuit_housing_2_length + circuit_housing_3_length + pmt_housing_length*0.5)*symmetry_axis, pmt_housing_logical, detector_name + "_pmt_housing", world_Logical, 0, 0, false);

	G4Tubs *pmt_housing_bottom_solid = new G4Tubs(detector_name + "_pmt_housing_solid",
			0., pmt_housing_radius - circuit_housing_thickness, circuit_housing_thickness*0.5, 0., twopi);
	G4LogicalVolume *pmt_housing_bottom_logical = new G4LogicalVolume(pmt_housing_bottom_solid, nist->FindOrBuildMaterial("G4_Al"), detector_name + "_pmt_housing_bottom_logical");
	pmt_housing_bottom_logical->SetVisAttributes(new G4VisAttributes(G4Color::Grey()));

	new G4PVPlacement(rotation, global_coordinates + (dist_from_center + crystal_housing_thickness + crystal_housing_case_length + circuit_housing_1_length + circuit_housing_2_length + circuit_housing_3_length + pmt_housing_length - circuit_housing_thickness*0.5)*symmetry_axis, pmt_housing_bottom_logical, detector_name + "_pmt_housing_bottom", world_Logical, 0, 0, false);

}