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

#include <string>

#include "G4PVPlacement.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "G4NistManager.hh"

#include "Detector.hh"

using std::string, std::to_string;

Detector::Detector(G4LogicalVolume *World_Logical, const G4String name)
: world_Logical(World_Logical),
  detector_name(name),
  rotation_matrix(nullptr)
{}

void Detector::Add_Filter(G4String filter_material, G4double filter_thickness, G4double filter_radius)
{
    filters.emplace_back(filter_material, filter_thickness, filter_radius);
}

void Detector::Add_Filter(Filter filter)
{
    filters.push_back(filter);
}

void Detector::Add_Wrap(G4String wrap_material, G4double wrap_thickness)
{
    wrap_materials.push_back(wrap_material);
    wrap_thicknesses.push_back(wrap_thickness);
}

double Detector::Construct_Filters(G4ThreeVector global_coordinates, double dist_from_center, double theta, double phi, double filter_position_z, const std::function<G4VSolid *(string, double, double)> &construct_solid)
{
    G4NistManager *nist = G4NistManager::Instance();
    G4ThreeVector e_r = unit_vector_r(theta, phi);
    /* G4ThreeVector e_theta = unit_vector_theta(theta, phi); */

    for(size_t i = 0; i < filters.size(); ++i) {
        const auto &filter = filters[i];
        string filter_solid_name = "filter_" + detector_name + "_" + to_string(i) + "_solid";
        auto *filter_solid = construct_solid(filter_solid_name, filter.radius, filter.thickness);

        string filter_logical_name = "filter_" + detector_name + "_" + to_string(i) + "_logical";
        G4LogicalVolume *filter_logical = new G4LogicalVolume(filter_solid,
            nist->FindOrBuildMaterial(filter.material),
            filter_logical_name);
        filter_logical->SetVisAttributes(
            new G4VisAttributes((i % 2) ? G4Color::Green() : G4Color::Red()));

        string filter_name = "filter_" + detector_name + "_" + to_string(i);
        new G4PVPlacement(rotation_matrix, global_coordinates + (dist_from_center - filter_position_z - filter.thickness * 0.5) * e_r, filter_logical, filter_name, world_Logical, 0, 0, false);
        filter_position_z = filter_position_z + filter.thickness;
    }
    return filter_position_z;
}

G4ThreeVector Detector::unit_vector_r(const double theta, const double phi) const
{
    return G4ThreeVector(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
}

G4ThreeVector Detector::unit_vector_theta(const double theta, const double phi) const
{
    return G4ThreeVector(cos(theta) * cos(phi), cos(theta) * sin(phi), -sin(theta));
}

G4ThreeVector Detector::unit_vector_phi([[maybe_unused]] const double theta, const double phi) const
{
    return G4ThreeVector(-sin(phi), cos(phi), 0.);
}

void Detector::rotate(const double theta, const double phi, const double alpha)
{

    rotation_matrix = new G4RotationMatrix();

    rotation_matrix->rotateZ(-phi);
    rotation_matrix->rotateY(-theta);
    if(alpha != 0.) {
        rotation_matrix->rotateZ(alpha);
    }
}
