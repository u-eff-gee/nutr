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

    Copyright (C) 2020-2022 Udo Friman-Gayer and Oliver Papst
*/

/**
 * This file contains code from the "utr" Geant4 simulation project.
 *
 * U. Friman-Gayer, J. Kleemann, and O. Papst, GEANT4 simulation of the Upstream
 * Target ROOM (UTR) at the HIγS facility (v2019.09), Zenodo (2019)
 * https://doi.org/10.5281/zenodo.3430154
 */

#include <algorithm>
#include <string>

#include "G4Color.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4Sphere.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4Torus.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4VisAttributes.hh"

#include "HPGe_Coaxial.hh"
#include "PLA.hh"

using std::string;
using std::to_string;

void HPGe_Coaxial::Construct_Detector(G4LogicalVolume *world_logical,
                                      G4ThreeVector global_coordinates) {

  G4NistManager *nist = G4NistManager::Instance();
  G4ThreeVector e_r = unit_vector_r(theta, phi);
  G4ThreeVector e_theta = unit_vector_theta(theta, phi);

  rotate(theta, phi, intrinsic_rotation_angle);

  /************* End cap *************/
  // End cap side
  double end_cap_inner_radius = properties.detector_radius +
                                properties.mount_cup_thickness +
                                properties.end_cap_to_crystal_gap_side;
  double end_cap_outer_radius =
      properties.detector_radius + properties.mount_cup_thickness +
      properties.end_cap_to_crystal_gap_side + properties.end_cap_thickness;
  double end_cap_side_length =
      properties.mount_cup_length + properties.end_cap_to_crystal_gap_front;

  G4Tubs *end_cap_side_solid =
      new G4Tubs(detector_name + "_end_cap_side_solid", end_cap_inner_radius,
                 end_cap_outer_radius, end_cap_side_length * 0.5, 0., twopi);
  G4LogicalVolume *end_cap_side_logical = new G4LogicalVolume(
      end_cap_side_solid,
      nist->FindOrBuildMaterial(properties.end_cap_material),
      detector_name + "_end_cap_side_logical");
  end_cap_side_logical->SetVisAttributes(new G4VisAttributes(G4Color::White()));
  new G4PVPlacement(rotation_matrix,
                    global_coordinates + (dist_from_center +
                                          properties.end_cap_window_thickness +
                                          end_cap_side_length * 0.5) *
                                             e_r,
                    end_cap_side_logical, detector_name + "_end_cap_side",
                    world_logical, 0, 0, false);

  // End cap window
  G4Tubs *end_cap_window_solid = new G4Tubs(
      detector_name + "_end_cap_window_solid", 0., end_cap_outer_radius,
      properties.end_cap_window_thickness * 0.5, 0., twopi);
  G4LogicalVolume *end_cap_window_logical = new G4LogicalVolume(
      end_cap_window_solid,
      nist->FindOrBuildMaterial(properties.end_cap_window_material),
      detector_name + "_end_cap_window_logical");
  end_cap_window_logical->SetVisAttributes(
      new G4VisAttributes(G4Color::White()));
  new G4PVPlacement(
      rotation_matrix,
      global_coordinates +
          (dist_from_center + properties.end_cap_window_thickness * 0.5) * e_r,
      end_cap_window_logical, detector_name + "_end_cap_window", world_logical,
      0, 0, false);

  // Vacuum inside end cap
  G4Tubs *end_cap_vacuum_solid =
      new G4Tubs(detector_name + "_end_cap_vacuum_solid", 0.,
                 end_cap_inner_radius, end_cap_side_length * 0.5, 0., twopi);
  G4LogicalVolume *end_cap_vacuum_logical = new G4LogicalVolume(
      end_cap_vacuum_solid, nist->FindOrBuildMaterial("G4_Galactic"),
      detector_name + "_end_cap_vacuum_logical");
  end_cap_vacuum_logical->SetVisAttributes(G4VisAttributes::GetInvisible());
  new G4PVPlacement(rotation_matrix,
                    global_coordinates + (dist_from_center +
                                          properties.end_cap_window_thickness +
                                          end_cap_side_length * 0.5) *
                                             e_r,
                    end_cap_vacuum_logical, detector_name + "_end_cap_vacuum",
                    world_logical, 0, 0, false);

  /************* Mount cup *************/
  // Mount cup side
  double mount_cup_inner_radius = properties.detector_radius;
  double mount_cup_outer_radius =
      properties.detector_radius + properties.mount_cup_thickness;
  double mount_cup_side_length = properties.mount_cup_length -
                                 properties.mount_cup_thickness -
                                 properties.mount_cup_base_thickness;

  G4Tubs *mount_cup_side_solid = new G4Tubs(
      detector_name + "_mount_cup_side_solid", mount_cup_inner_radius,
      mount_cup_outer_radius, mount_cup_side_length * 0.5, 0., twopi);
  G4LogicalVolume *mount_cup_side_logical = new G4LogicalVolume(
      mount_cup_side_solid,
      nist->FindOrBuildMaterial(properties.mount_cup_material),
      detector_name + "_mount_cup_side_logical");
  mount_cup_side_logical->SetVisAttributes(
      new G4VisAttributes(G4Color::Cyan()));
  new G4PVPlacement(0,
                    G4ThreeVector(0., 0.,
                                  -end_cap_side_length * 0.5 +
                                      properties.end_cap_to_crystal_gap_front +
                                      properties.mount_cup_thickness +
                                      mount_cup_side_length * 0.5),
                    mount_cup_side_logical, detector_name + "_mount_cup_side",
                    end_cap_vacuum_logical, 0, 0, false);

  // Mount cup face
  G4Tubs *mount_cup_face_solid = new G4Tubs(
      detector_name + "_mount_cup_face_solid", 0., mount_cup_outer_radius,
      properties.mount_cup_thickness * 0.5, 0., twopi);
  G4LogicalVolume *mount_cup_face_logical = new G4LogicalVolume(
      mount_cup_face_solid,
      nist->FindOrBuildMaterial(properties.mount_cup_material),
      detector_name + "_mount_cup_face_logical");
  mount_cup_face_logical->SetVisAttributes(
      new G4VisAttributes(G4Color::Cyan()));
  new G4PVPlacement(0,
                    G4ThreeVector(0., 0.,
                                  -end_cap_side_length * 0.5 +
                                      properties.end_cap_to_crystal_gap_front +
                                      properties.mount_cup_thickness * 0.5),
                    mount_cup_face_logical, detector_name + "_mount_cup_face",
                    end_cap_vacuum_logical, 0, 0, false);

  // Mount cup base
  G4Tubs *mount_cup_base_solid =
      new G4Tubs(detector_name + "_mount_cup_base_solid",
                 properties.hole_radius, mount_cup_outer_radius,
                 properties.mount_cup_base_thickness * 0.5, 0., twopi);
  G4LogicalVolume *mount_cup_base_logical = new G4LogicalVolume(
      mount_cup_base_solid,
      nist->FindOrBuildMaterial(properties.mount_cup_material),
      detector_name + "_mount_cup_base_logical");
  mount_cup_base_logical->SetVisAttributes(
      new G4VisAttributes(G4Color::Cyan()));
  new G4PVPlacement(
      0,
      G4ThreeVector(0., 0.,
                    -end_cap_side_length * 0.5 +
                        properties.end_cap_to_crystal_gap_front +
                        properties.mount_cup_thickness + mount_cup_side_length +
                        0.5 * properties.mount_cup_base_thickness),
      mount_cup_base_logical, detector_name + "_mount_cup_base",
      end_cap_vacuum_logical, 0, 0, false);

  /************* Cold finger *************/

  double cold_finger_length =
      properties.cold_finger_penetration_depth + mount_cup_side_length +
      properties.mount_cup_base_thickness - properties.detector_length;

  G4Tubs *cold_finger_shaft_solid = new G4Tubs(
      detector_name + "_cold_finger_shaft_solid", 0.,
      properties.cold_finger_radius,
      0.5 * (cold_finger_length - properties.cold_finger_radius), 0., twopi);
  G4Sphere *cold_finger_tip_solid =
      new G4Sphere(detector_name + "cold_finger_tip_solid", 0.,
                   properties.cold_finger_radius, 0., twopi, 0., pi);
  G4UnionSolid *cold_finger_solid = new G4UnionSolid(
      detector_name + "cold_finger_solid", cold_finger_shaft_solid,
      cold_finger_tip_solid, 0,
      G4ThreeVector(
          0., 0., -0.5 * (cold_finger_length - properties.cold_finger_radius)));

  G4LogicalVolume *cold_finger_logical = new G4LogicalVolume(
      cold_finger_solid,
      nist->FindOrBuildMaterial(properties.cold_finger_material),
      detector_name + "_cold_finger_logical", 0, 0, 0);

  cold_finger_logical->SetVisAttributes(
      new G4VisAttributes(G4Color(1.0, 0.5, 0.0)));

  new G4PVPlacement(
      0,
      G4ThreeVector(0., 0.,
                    0.5 * (end_cap_side_length - cold_finger_length +
                           properties.cold_finger_radius)),
      cold_finger_logical, detector_name + "_cold_finger",
      end_cap_vacuum_logical, 0, 0, false);

  /************* Detector crystal *************/

  G4Tubs *crystal_tube_solid = new G4Tubs(
      detector_name + "crystal_tube_solid", 0., properties.detector_radius,
      0.5 * (properties.detector_length - properties.detector_face_radius), 0.,
      twopi);

  G4Torus *crystal_rim_solid = new G4Torus(
      detector_name + "crystal_rim_solid", 0., properties.detector_face_radius,
      properties.detector_radius - properties.detector_face_radius, 0., twopi);
  G4Tubs *crystal_front_solid =
      new G4Tubs(detector_name + "crystal_front_solid", 0.,
                 properties.detector_radius - properties.detector_face_radius,
                 properties.detector_face_radius, 0., twopi);

  G4UnionSolid *crystal_tube_with_rim_solid =
      new G4UnionSolid(detector_name + "crystal_tube_with_rim_solid",
                       crystal_tube_solid, crystal_rim_solid, 0,
                       G4ThreeVector(0., 0.,
                                     -0.5 * (properties.detector_length -
                                             properties.detector_face_radius)));

  G4UnionSolid *crystal_full_solid =
      new G4UnionSolid(detector_name + "crystal_new_solid",
                       crystal_tube_with_rim_solid, crystal_front_solid, 0,
                       G4ThreeVector(0., 0.,
                                     -0.5 * (properties.detector_length -
                                             properties.detector_face_radius)));

  G4Tubs *crystal_hole_shaft_solid = new G4Tubs(
      detector_name + "_crystal_hole_shaft_solid", 0., properties.hole_radius,
      0.5 * (properties.hole_depth - properties.hole_radius), 0., twopi);
  G4Sphere *crystal_hole_tip_solid =
      new G4Sphere(detector_name + "_crystal_hole_tip_solid", 0.,
                   properties.hole_radius, 0., twopi, 0., pi);
  G4UnionSolid *crystal_hole_solid = new G4UnionSolid(
      detector_name + "_crystal_hole_solid", crystal_hole_shaft_solid,
      crystal_hole_tip_solid, 0,
      G4ThreeVector(0., 0.,
                    -0.5 * (properties.hole_depth - properties.hole_radius)));

  G4SubtractionSolid *crystal_solid = new G4SubtractionSolid(
      detector_name + "crystal_solid", crystal_full_solid, crystal_hole_solid,
      0,
      G4ThreeVector(0., 0.,
                    0.5 * (properties.detector_length -
                           properties.detector_face_radius -
                           properties.hole_depth + properties.hole_radius)));

  G4LogicalVolume *crystal_logical =
      new G4LogicalVolume(crystal_solid, nist->FindOrBuildMaterial("G4_Ge"),
                          detector_name + "_logical", 0, 0, 0);

  crystal_logical->SetVisAttributes(new G4VisAttributes(G4Color::Green()));

  new G4PVPlacement(0,
                    G4ThreeVector(0., 0.,
                                  -end_cap_side_length * 0.5 +
                                      properties.end_cap_to_crystal_gap_front +
                                      properties.mount_cup_thickness +
                                      0.5 * (properties.detector_length +
                                             properties.detector_face_radius)),
                    crystal_logical, detector_name + "_crystal",
                    end_cap_vacuum_logical, 0, 0, false);

  /************* Detector crystal (active volume) *************/

  G4Tubs *crystal_active_tube_solid = new G4Tubs(
      detector_name + "crystal_active_tube_solid", 0.,
      (1. - dead_layer[0]) * properties.detector_radius,
      0.5 * (1. - dead_layer[0]) *
          (properties.detector_length - properties.detector_face_radius),
      0., twopi);

  G4Torus *crystal_active_rim_solid =
      new G4Torus(detector_name + "crystal_active_rim_solid", 0.,
                  (1. - dead_layer[0]) * properties.detector_face_radius,
                  (1. - dead_layer[0]) * (properties.detector_radius -
                                          properties.detector_face_radius),
                  0., twopi);
  G4Tubs *crystal_active_front_solid = new G4Tubs(
      detector_name + "crystal_active_front_solid", 0.,
      (1. - dead_layer[0]) *
          (properties.detector_radius - properties.detector_face_radius),
      (1. - dead_layer[0]) * properties.detector_face_radius, 0., twopi);

  G4UnionSolid *crystal_active_tube_with_rim_solid =
      new G4UnionSolid(detector_name + "crystal_active_tube_with_rim_solid",
                       crystal_active_tube_solid, crystal_active_rim_solid, 0,
                       G4ThreeVector(0., 0.,
                                     -0.5 * (1. - dead_layer[0]) *
                                         (properties.detector_length -
                                          properties.detector_face_radius)));

  G4UnionSolid *crystal_active_full_solid = new G4UnionSolid(
      detector_name + "crystal_active_full_solid",
      crystal_active_tube_with_rim_solid, crystal_active_front_solid, 0,
      G4ThreeVector(
          0., 0.,
          -0.5 * (1. - dead_layer[0]) *
              (properties.detector_length - properties.detector_face_radius)));

  G4Tubs *crystal_active_hole_shaft_solid =
      new G4Tubs(detector_name + "_crystal_active_hole_shaft_solid", 0.,
                 (1. + dead_layer[0]) * properties.hole_radius,
                 0.5 * (1. + dead_layer[0]) *
                     (properties.hole_depth - properties.hole_radius),
                 0., twopi);
  G4Sphere *crystal_active_hole_tip_solid = new G4Sphere(
      detector_name + "_crystal_active_hole_tip_solid", 0.,
      (1. + dead_layer[0]) * properties.hole_radius, 0., twopi, 0., pi);
  G4UnionSolid *crystal_active_hole_solid = new G4UnionSolid(
      detector_name + "_crystal_active_hole_solid",
      crystal_active_hole_shaft_solid, crystal_active_hole_tip_solid, 0,
      G4ThreeVector(0., 0.,
                    -0.5 * (1. + dead_layer[0]) *
                        (properties.hole_depth - properties.hole_radius)));

  G4SubtractionSolid *crystal_active_solid = new G4SubtractionSolid(
      detector_name + "_crystal_active_solid", crystal_active_full_solid,
      crystal_active_hole_solid, 0,
      G4ThreeVector(
          0., 0.,
          0.5 * ((1. - dead_layer[0]) * properties.detector_length -
                 (1. - dead_layer[0]) * properties.detector_face_radius -
                 (1. + dead_layer[0]) * properties.hole_depth +
                 (1. + dead_layer[0]) * properties.hole_radius)));

  sensitive_logical_volumes.push_back(new G4LogicalVolume(
      crystal_active_solid, nist->FindOrBuildMaterial("G4_Ge"), detector_name,
      0, 0, 0));

  sensitive_logical_volumes[0]->SetVisAttributes(
      new G4VisAttributes(G4Color::Green()));

  new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), sensitive_logical_volumes[0],
                    detector_name + "_crystal_active", crystal_logical, 0, 0,
                    false);

  if (dewar_properties.dewar_material != "") {
    /************* Connection dewar-detector *************/
    G4Tubs *connection_solid =
        new G4Tubs(detector_name + "_connection_solid", 0.,
                   dewar_properties.connection_radius,
                   dewar_properties.connection_length * 0.5, 0., twopi);
    G4LogicalVolume *connection_logical = new G4LogicalVolume(
        connection_solid,
        nist->FindOrBuildMaterial(dewar_properties.connection_material),
        detector_name + "_connection_logical");
    connection_logical->SetVisAttributes(new G4VisAttributes(G4Color::White()));
    new G4PVPlacement(
        rotation_matrix,
        global_coordinates +
            (dist_from_center + properties.end_cap_window_thickness +
             end_cap_side_length + dewar_properties.connection_length * 0.5) *
                e_r,
        connection_logical, detector_name + "_connection", world_logical, 0, 0,
        false);

    if (intrinsic_rotation_angle != 0.)
      e_theta.rotate(intrinsic_rotation_angle, e_r);

    /************* Dewar *************/
    double dewar_side_length = dewar_properties.dewar_length -
                               2. * dewar_properties.dewar_wall_thickness;

    // Dewar face
    G4Tubs *dewar_face_solid =
        new G4Tubs(detector_name + "_dewar_face_solid", 0.,
                   dewar_properties.dewar_outer_radius,
                   dewar_properties.dewar_wall_thickness * 0.5, 0., twopi);
    G4LogicalVolume *dewar_face_logical = new G4LogicalVolume(
        dewar_face_solid,
        nist->FindOrBuildMaterial(dewar_properties.dewar_material),
        detector_name + "_dewar_face_logical");
    dewar_face_logical->SetVisAttributes(new G4VisAttributes(G4Color::Brown()));
    new G4PVPlacement(
        rotation_matrix,
        global_coordinates +
            (dist_from_center + properties.end_cap_window_thickness +
             end_cap_side_length + dewar_properties.connection_length +
             dewar_properties.dewar_wall_thickness * 0.5) *
                e_r +
            dewar_properties.dewar_offset * e_theta,
        dewar_face_logical, detector_name + "_dewar_face", world_logical, 0, 0,
        false);

    // Dewar side
    G4Tubs *dewar_side_solid =
        new G4Tubs(detector_name + "_dewar_side_solid",
                   dewar_properties.dewar_outer_radius -
                       dewar_properties.dewar_wall_thickness,
                   dewar_properties.dewar_outer_radius, dewar_side_length * 0.5,
                   0., twopi);
    G4LogicalVolume *dewar_side_logical = new G4LogicalVolume(
        dewar_side_solid,
        nist->FindOrBuildMaterial(dewar_properties.dewar_material),
        detector_name + "_dewar_side_logical");
    dewar_side_logical->SetVisAttributes(new G4VisAttributes(G4Color::Brown()));
    new G4PVPlacement(
        rotation_matrix,
        global_coordinates +
            (dist_from_center + properties.end_cap_window_thickness +
             end_cap_side_length + dewar_properties.connection_length +
             dewar_properties.dewar_wall_thickness + dewar_side_length * 0.5) *
                e_r +
            dewar_properties.dewar_offset * e_theta,
        dewar_side_logical, detector_name + "_dewar_side", world_logical, 0, 0,
        false);

    // Dewar base
    G4Tubs *dewar_base_solid =
        new G4Tubs(detector_name + "_dewar_base_solid", 0.,
                   dewar_properties.dewar_outer_radius,
                   dewar_properties.dewar_wall_thickness * 0.5, 0., twopi);
    G4LogicalVolume *dewar_base_logical = new G4LogicalVolume(
        dewar_base_solid,
        nist->FindOrBuildMaterial(dewar_properties.dewar_material),
        detector_name + "_dewar_base_logical");
    dewar_base_logical->SetVisAttributes(new G4VisAttributes(G4Color::Brown()));
    new G4PVPlacement(
        rotation_matrix,
        global_coordinates +
            (dist_from_center + properties.end_cap_window_thickness +
             end_cap_side_length + dewar_properties.connection_length +
             dewar_properties.dewar_wall_thickness * 1.5 + dewar_side_length) *
                e_r +
            dewar_properties.dewar_offset * e_theta,
        dewar_base_logical, detector_name + "_dewar_base", world_logical, 0, 0,
        false);
  }

  // Wraps
  if (wraps.size()) {
    double wrap_radius =
        end_cap_outer_radius; // Will be gradually increased to be able to place
    // wraps on top of each other
    G4Tubs *wrap_solid = nullptr;
    G4LogicalVolume *wrap_logical = nullptr;
    string wrap_solid_name, wrap_logical_name, wrap_name;
    for (unsigned int i = 0; i < wraps.size(); ++i) {
      wrap_solid_name = "wrap_" + detector_name + "_" + to_string(i) + "_solid";
      wrap_solid = new G4Tubs(wrap_solid_name, wrap_radius,
                              wrap_radius + wraps[i].thickness,
                              properties.end_cap_length * 0.5, 0., twopi);
      wrap_solid_name.clear();

      wrap_logical_name =
          "wrap_" + detector_name + "_" + to_string(i) + "_logical";
      wrap_logical = new G4LogicalVolume(
          wrap_solid, nist->FindOrBuildMaterial(wraps[i].material),
          wrap_logical_name);
      wrap_logical_name.clear();
      wrap_logical->SetVisAttributes(new G4VisAttributes(G4Color::Green()));

      wrap_name = "wrap_" + detector_name + "_" + to_string(i);
      new G4PVPlacement(
          rotation_matrix,
          global_coordinates +
              (dist_from_center + properties.end_cap_length * 0.5) * e_r,
          wrap_logical, wrap_name, world_logical, 0, 0, false);
      wrap_name.clear();
      wrap_radius = wrap_radius + wraps[i].thickness;
    }
  }
}

void HPGe_Coaxial::Construct_Filter_Case(G4LogicalVolume *world_logical,
                                         G4ThreeVector global_coordinates,
                                         double filter_dist_from_center) {
  // The part modeled here does not exist in reality, but may be 3D printed in
  // the future. The dimensions are estimated from similar cases for the
  // LaBr3Ce_3x2 and CeBr3_2x2 classes.
  PLA pla;

  const G4ThreeVector e_r = unit_vector_r(theta, phi);

  const double filter_case_inner_radius = properties.end_cap_outer_radius;
  const double filter_case_wall_thickness = 5. * mm;
  const double filter_case_length = 40. * mm;
  const double filter_case_front_inner_radius =
      properties.end_cap_outer_radius - 4. * mm;
  const double filter_case_front_length = 3. * mm;

  G4Tubs *filter_case_front_solid =
      new G4Tubs(detector_name + "_filter_case_front_solid",
                 filter_case_front_inner_radius,
                 filter_case_inner_radius + filter_case_wall_thickness,
                 filter_case_front_length * 0.5, 0., twopi);
  G4LogicalVolume *filter_case_front_logical = new G4LogicalVolume(
      filter_case_front_solid, G4Material::GetMaterial("PLA"),
      detector_name + "_filter_case_front_logical");
  G4RotationMatrix *rotation = new G4RotationMatrix();
  rotation->rotateZ(-phi);
  rotation->rotateY(-theta);
  filter_case_front_logical->SetVisAttributes(
      new G4VisAttributes(G4Color::Blue()));
  new G4PVPlacement(
      rotation,
      global_coordinates +
          (filter_dist_from_center - 0.5 * filter_case_front_length) * e_r,
      filter_case_front_logical, detector_name + "_filter_case_front",
      world_logical, 0, 0, false);

  G4Tubs *filter_case_solid = new G4Tubs(
      detector_name + "_filter_case_solid", filter_case_inner_radius,
      filter_case_inner_radius + filter_case_wall_thickness,
      (filter_case_length - filter_case_front_length) * 0.5, 0., twopi);
  G4LogicalVolume *filter_case_logical =
      new G4LogicalVolume(filter_case_solid, G4Material::GetMaterial("PLA"),
                          detector_name + "_filter_case_logical");
  filter_case_logical->SetVisAttributes(new G4VisAttributes(G4Color::Blue()));
  new G4PVPlacement(rotation,
                    global_coordinates + (filter_dist_from_center +
                                          0.5 * (filter_case_length -
                                                 filter_case_front_length)) *
                                             e_r,
                    filter_case_logical, detector_name + "_filter_case",
                    world_logical, 0, 0, false);
}

G4VSolid *HPGe_Coaxial::Filter_Shape(const string name,
                                     const Filter &filter) const {
  return new G4Tubs(name, 0., filter.radius, filter.thickness * 0.5, 0., twopi);
}