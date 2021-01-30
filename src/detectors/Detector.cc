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

#include "Detector.hh"

Detector::Detector(G4LogicalVolume *World_Logical, const G4String name):
	world_Logical(World_Logical),
	detector_name(name),
	rotation_matrix(nullptr)
{}

void Detector::Add_Filter(G4String filter_material, G4double filter_thickness, G4double filter_radius){
	filter_materials.push_back(filter_material);
	filter_thicknesses.push_back(filter_thickness);
	filter_radii.push_back(filter_radius);
}

void Detector::Add_Wrap(G4String wrap_material, G4double wrap_thickness){
	wrap_materials.push_back(wrap_material);
	wrap_thicknesses.push_back(wrap_thickness);
}

G4ThreeVector Detector::unit_vector_r(const double theta, const double phi) const {
	return G4ThreeVector(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta));
}

G4ThreeVector Detector::unit_vector_theta(const double theta, const double phi) const {
	return G4ThreeVector(cos(theta)*cos(phi), cos(theta)*sin(phi), -sin(theta));
}

G4ThreeVector Detector::unit_vector_phi([[maybe_unused]] const double theta, const double phi) const {
	return G4ThreeVector(-sin(phi), cos(phi), 0.);
}

void Detector::rotate(const double theta, const double phi, const double alpha){
	
	rotation_matrix = new G4RotationMatrix();

	rotation_matrix->rotateZ(-phi);
	rotation_matrix->rotateY(-theta);
	if(alpha != 0.){
		rotation_matrix->rotateZ(alpha);
	}
}