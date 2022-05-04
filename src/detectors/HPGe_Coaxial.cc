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
#include "G4Polycone.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

#include "HPGe_Coaxial.hh"
#include "OptimizePolycone.hh"

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
  G4double end_cap_inner_radius = properties.detector_radius +
                                  properties.mount_cup_thickness +
                                  properties.end_cap_to_crystal_gap_side;
  G4double end_cap_outer_radius =
      properties.detector_radius + properties.mount_cup_thickness +
      properties.end_cap_to_crystal_gap_side + properties.end_cap_thickness;
  G4double end_cap_side_length =
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
  G4double mount_cup_inner_radius = properties.detector_radius;
  G4double mount_cup_outer_radius =
      properties.detector_radius + properties.mount_cup_thickness;
  G4double mount_cup_side_length = properties.mount_cup_length -
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

  G4double cold_finger_length =
      properties.cold_finger_penetration_depth + mount_cup_side_length +
      properties.mount_cup_base_thickness - properties.detector_length;

  const G4int nsteps = 500;

  G4double zPlaneTemp[nsteps];
  G4double rInnerTemp[nsteps];
  G4double rOuterTemp[nsteps];

  G4double z;

  for (int i = 0; i < nsteps; i++) {
    z = (1. - (double)i / (nsteps - 1)) * cold_finger_length;

    zPlaneTemp[i] = z;

    rInnerTemp[i] = 0. * mm;

    if (z >= properties.cold_finger_radius) {
      rOuterTemp[i] = properties.cold_finger_radius;
    } else if (z >= 0.) {
      rOuterTemp[i] = properties.cold_finger_radius *
                      sqrt(1. - pow((z - properties.cold_finger_radius) /
                                        properties.cold_finger_radius,
                                    2));
    } else {
      rOuterTemp[i] = 0. * mm;
    }
  }

  G4double zPlane[nsteps];
  G4double rInner[nsteps];
  G4double rOuter[nsteps];

  OptimizePolycone *opt = new OptimizePolycone();
  G4int nsteps_optimized =
      opt->Optimize(zPlaneTemp, rInnerTemp, rOuterTemp, zPlane, rInner, rOuter,
                    nsteps, detector_name + "_cold_finger_solid");

  G4Polycone *cold_finger_solid =
      new G4Polycone(detector_name + "_cold_finger_solid", 0. * deg, 360. * deg,
                     nsteps_optimized, zPlane, rInner, rOuter);

  G4LogicalVolume *cold_finger_logical = new G4LogicalVolume(
      cold_finger_solid,
      nist->FindOrBuildMaterial(properties.cold_finger_material),
      detector_name + "_cold_finger_logical", 0, 0, 0);

  cold_finger_logical->SetVisAttributes(
      new G4VisAttributes(G4Color(1.0, 0.5, 0.0)));

  new G4PVPlacement(
      0, G4ThreeVector(0., 0., end_cap_side_length * 0.5 - cold_finger_length),
      cold_finger_logical, detector_name + "_cold_finger",
      end_cap_vacuum_logical, 0, 0, false);

  /************* Detector crystal *************/

  for (int i = 0; i < nsteps; i++) {
    z = (1. - (double)i / (nsteps - 1)) * properties.detector_length;

    zPlaneTemp[i] = z;

    // rInnerTemp[i] = 0. * mm;
    if (z >= properties.detector_length - properties.hole_depth) {
      if (z >= properties.detector_length - properties.hole_depth +
                   properties.hole_radius) {
        rInnerTemp[i] = properties.hole_radius;
      } else {
        rInnerTemp[i] =
            properties.hole_radius *
            sqrt(1. -
                 pow((z - (properties.detector_length - properties.hole_depth +
                           properties.hole_radius)) /
                         properties.hole_radius,
                     2));
      }
    } else {
      rInnerTemp[i] = 0.;
    }

    if (z >= properties.detector_face_radius) {
      rOuterTemp[i] = properties.detector_radius;
    } else if (z >= 0.) {
      rOuterTemp[i] =
          properties.detector_face_radius *
              sqrt(1. - pow((z - properties.detector_face_radius) /
                                properties.detector_face_radius,
                            2)) +
          (properties.detector_radius - properties.detector_face_radius);
    } else {
      rOuterTemp[i] = 0. * mm;
    }
  }

  nsteps_optimized =
      opt->Optimize(zPlaneTemp, rInnerTemp, rOuterTemp, zPlane, rInner, rOuter,
                    nsteps, detector_name + "_crystal_solid");

  G4Polycone *crystal_solid =
      new G4Polycone("crystal_solid", 0. * deg, 360. * deg, nsteps_optimized,
                     zPlane, rInner, rOuter);

  sensitive_logical_volumes.push_back(
      new G4LogicalVolume(crystal_solid, nist->FindOrBuildMaterial("G4_Ge"),
                          detector_name, 0, 0, 0));

  sensitive_logical_volumes[0]->SetVisAttributes(
      new G4VisAttributes(G4Color::Green()));

  new G4PVPlacement(0,
                    G4ThreeVector(0., 0.,
                                  -end_cap_side_length * 0.5 +
                                      properties.end_cap_to_crystal_gap_front +
                                      properties.mount_cup_thickness),
                    sensitive_logical_volumes[0], detector_name + "_crystal",
                    end_cap_vacuum_logical, 0, 0, false);

  if (use_dewar) {
    /************* Connection dewar-detector *************/
    G4Tubs *connection_solid = new G4Tubs(
        detector_name + "connection_solid", 0., properties.connection_radius,
        properties.connection_length * 0.5, 0., twopi);
    G4LogicalVolume *connection_logical = new G4LogicalVolume(
        connection_solid,
        nist->FindOrBuildMaterial(properties.connection_material),
        detector_name + "connection_logical");
    connection_logical->SetVisAttributes(new G4VisAttributes(G4Color::White()));
    new G4PVPlacement(
        rotation_matrix,
        global_coordinates +
            (dist_from_center + properties.end_cap_window_thickness +
             end_cap_side_length + properties.connection_length * 0.5) *
                e_r,
        connection_logical, detector_name + "connection", world_logical, 0, 0,
        false);

    if (intrinsic_rotation_angle != 0.)
      e_theta.rotate(intrinsic_rotation_angle, e_r);

    /************* Dewar *************/
    G4double dewar_side_length =
        properties.dewar_length - 2. * properties.dewar_wall_thickness;

    // Dewar face
    G4Tubs *dewar_face_solid = new G4Tubs(
        detector_name + "_dewar_face_solid", 0., properties.dewar_outer_radius,
        properties.dewar_wall_thickness * 0.5, 0., twopi);
    G4LogicalVolume *dewar_face_logical = new G4LogicalVolume(
        dewar_face_solid, nist->FindOrBuildMaterial(properties.dewar_material),
        detector_name + "_dewar_face_logical");
    dewar_face_logical->SetVisAttributes(new G4VisAttributes(G4Color::Brown()));
    new G4PVPlacement(rotation_matrix,
                      global_coordinates +
                          (dist_from_center +
                           properties.end_cap_window_thickness +
                           end_cap_side_length + properties.connection_length +
                           properties.dewar_wall_thickness * 0.5) *
                              e_r +
                          properties.dewar_offset * e_theta,
                      dewar_face_logical, detector_name + "_dewar_face",
                      world_logical, 0, 0, false);

    // Dewar side
    G4Tubs *dewar_side_solid = new G4Tubs(
        detector_name + "_dewar_side_solid",
        properties.dewar_outer_radius - properties.dewar_wall_thickness,
        properties.dewar_outer_radius, dewar_side_length * 0.5, 0., twopi);
    G4LogicalVolume *dewar_side_logical = new G4LogicalVolume(
        dewar_side_solid, nist->FindOrBuildMaterial(properties.dewar_material),
        detector_name + "_dewar_side_logical");
    dewar_side_logical->SetVisAttributes(new G4VisAttributes(G4Color::Brown()));
    new G4PVPlacement(
        rotation_matrix,
        global_coordinates +
            (dist_from_center + properties.end_cap_window_thickness +
             end_cap_side_length + properties.connection_length +
             properties.dewar_wall_thickness + dewar_side_length * 0.5) *
                e_r +
            properties.dewar_offset * e_theta,
        dewar_side_logical, detector_name + "_dewar_side", world_logical, 0, 0,
        false);

    // Dewar base
    G4Tubs *dewar_base_solid = new G4Tubs(
        detector_name + "_dewar_base_solid", 0., properties.dewar_outer_radius,
        properties.dewar_wall_thickness * 0.5, 0., twopi);
    G4LogicalVolume *dewar_base_logical = new G4LogicalVolume(
        dewar_base_solid, nist->FindOrBuildMaterial(properties.dewar_material),
        detector_name + "_dewar_base_logical");
    dewar_base_logical->SetVisAttributes(new G4VisAttributes(G4Color::Brown()));
    new G4PVPlacement(
        rotation_matrix,
        global_coordinates +
            (dist_from_center + properties.end_cap_window_thickness +
             end_cap_side_length + properties.connection_length +
             properties.dewar_wall_thickness * 1.5 + dewar_side_length) *
                e_r +
            properties.dewar_offset * e_theta,
        dewar_base_logical, detector_name + "_dewar_base", world_logical, 0, 0,
        false);
  }

  // Wraps
  if (wraps.size()) {
    G4double wrap_radius =
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

G4VSolid *HPGe_Coaxial::Filter_Shape(const string name,
                                     const Filter &filter) const {
  return new G4Tubs(name, 0., filter.radius, filter.thickness * 0.5, 0., twopi);
}