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

#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

#include "Detector.hh"

using std::string, std::to_string;

Detector::Detector(const G4String _name, const G4double _theta,
                   const G4double _phi, const G4double _dist_from_center,
                   const vector<Filter> _filters, const vector<Filter> _wraps,
                   G4double _intrinsic_rotation_angle,
                   const double _default_filter_radius)
    : detector_name(_name), theta(_theta), phi(_phi),
      dist_from_center(_dist_from_center), filters(_filters), wraps(_wraps),
      intrinsic_rotation_angle(_intrinsic_rotation_angle),
      default_filter_radius(_default_filter_radius), rotation_matrix(nullptr) {}

void Detector::Construct(G4LogicalVolume *world_logical,
                         G4ThreeVector global_coordinates) {
  Construct_Detector(world_logical, global_coordinates);
  const double filter_dist_from_center =
      Construct_Filters(world_logical, global_coordinates);
  Construct_Filter_Case(world_logical, global_coordinates,
                        filter_dist_from_center);
}

double Detector::Construct_Filters(G4LogicalVolume *world_logical,
                                   G4ThreeVector global_coordinates) {
  G4NistManager *nist = G4NistManager::Instance();
  G4ThreeVector e_r = unit_vector_r(theta, phi);
  double filter_position_z = 0.;

  rotate(theta, phi, intrinsic_rotation_angle);

  for (size_t i = 0; i < filters.size(); ++i) {
    const auto &filter = filters[i];
    string filter_solid_name =
        "filter_" + detector_name + "_" + to_string(i) + "_solid";
    G4VSolid *filter_solid = nullptr;
    if (filter.use_default_radius) {
      filter_solid = Filter_Shape(
          filter_solid_name,
          Filter(filter.material, filter.thickness, default_filter_radius));
    } else {
      filter_solid = Filter_Shape(filter_solid_name, filter);
    }

    string filter_logical_name =
        "filter_" + detector_name + "_" + to_string(i) + "_logical";
    G4LogicalVolume *filter_logical = new G4LogicalVolume(
        filter_solid, nist->FindOrBuildMaterial(filter.material),
        filter_logical_name);
    filter_logical->SetVisAttributes(
        new G4VisAttributes((i % 2) ? G4Color::Green() : G4Color::Red()));

    string filter_name = "filter_" + detector_name + "_" + to_string(i);
    new G4PVPlacement(
        rotation_matrix,
        global_coordinates +
            (dist_from_center - filter_position_z - filter.thickness * 0.5) *
                e_r,
        filter_logical, filter_name, world_logical, 0, 0, false);
    filter_position_z = filter_position_z + filter.thickness;
  }
  return filter_position_z;
}

G4ThreeVector Detector::unit_vector_r(const double _theta,
                                      const double _phi) const {
  return G4ThreeVector(sin(_theta) * cos(_phi), sin(_theta) * sin(_phi),
                       cos(_theta));
}

G4ThreeVector Detector::unit_vector_theta(const double _theta,
                                          const double _phi) const {
  return G4ThreeVector(cos(_theta) * cos(_phi), cos(_theta) * sin(_phi),
                       -sin(_theta));
}

G4ThreeVector Detector::unit_vector_phi([[maybe_unused]] const double _theta,
                                        const double _phi) const {
  return G4ThreeVector(-sin(_phi), cos(_phi), 0.);
}

void Detector::rotate(const double _theta, const double _phi,
                      const double alpha) {

  rotation_matrix = new G4RotationMatrix();

  rotation_matrix->rotateZ(-_phi);
  rotation_matrix->rotateY(-_theta);
  if (alpha != 0.) {
    rotation_matrix->rotateZ(alpha);
  }
}
