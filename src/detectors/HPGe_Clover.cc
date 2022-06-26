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

/*
 * General topology of a clover detector was taken from an
 * Eurysis manual for clover detectors.
 */

#include <string>

using std::to_string;

#include <utility>

#include "G4Box.hh"
#include "G4Color.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

#include "HPGe_Clover.hh"
#include "PLA.hh"

void HPGe_Clover::Construct_Detector(G4LogicalVolume *world_logical,
                                     G4ThreeVector global_coordinates) {

  G4NistManager *nist = G4NistManager::Instance();
  G4ThreeVector symmetry_axis(
      sin(theta) * cos(phi), sin(theta) * sin(phi),
      cos(theta)); // Symmetry axis along which the single elements of the
                   // detector are constructed

  G4RotationMatrix *rotation = new G4RotationMatrix();
  rotation->rotateZ(-phi);
  rotation->rotateY(-theta);
  if (intrinsic_rotation_angle != 0.) {
    rotation->rotateZ(intrinsic_rotation_angle);
  }

  /******** Front end cap *********/

  G4VSolid *end_cap_front_solid =
      rounded_box("end_cap_front_solid", properties.end_cap_front_side_length,
                  properties.end_cap_front_length,
                  properties.end_cap_front_rounding_radius, 20);
  G4LogicalVolume *end_cap_front_logical = new G4LogicalVolume(
      end_cap_front_solid,
      nist->FindOrBuildMaterial(properties.end_cap_material),
      detector_name + "_end_cap_front_logical");
  new G4PVPlacement(
      rotation,
      global_coordinates +
          (dist_from_center + 0.5 * properties.end_cap_front_length) *
              symmetry_axis,
      end_cap_front_logical, detector_name + "end_cap_front", world_logical, 0,
      0, false);

  /******** Vacuum around crystal ********/

  G4VSolid *vacuum_solid = rounded_box(
      "_vacuum_solid",
      properties.end_cap_front_side_length -
          2. * properties.end_cap_front_thickness,
      properties.vacuum_length, properties.end_cap_front_rounding_radius, 20);

  G4LogicalVolume *vacuum_logical = new G4LogicalVolume(
      vacuum_solid, nist->FindOrBuildMaterial("G4_Galactic"),
      detector_name + "_vacuum_logical");
  vacuum_logical->SetVisAttributes(G4Color::Cyan());
  new G4PVPlacement(0,
                    G4ThreeVector(0., 0.,
                                  -0.5 * properties.end_cap_front_length +
                                      properties.end_cap_window_thickness +
                                      0.5 * properties.vacuum_length),
                    vacuum_logical, detector_name + "_vacuum",
                    end_cap_front_logical, 0, 0, false);

  /******** Air at the back of the front end cap ********/

  G4VSolid *air_front_solid = rounded_box(
      "_air_front_solid",
      properties.end_cap_front_side_length -
          2. * properties.end_cap_front_thickness,
      properties.end_cap_front_length - properties.end_cap_window_thickness -
          properties.end_cap_front_thickness - properties.vacuum_length,
      properties.end_cap_front_rounding_radius, 20);

  G4LogicalVolume *air_front_logical =
      new G4LogicalVolume(air_front_solid, nist->FindOrBuildMaterial("G4_AIR"),
                          detector_name + "_air_front_logical");
  air_front_logical->SetVisAttributes(G4Color::Red());
  new G4PVPlacement(
      0,
      G4ThreeVector(0., 0.,
                    -0.5 * properties.end_cap_front_length +
                        properties.end_cap_window_thickness +
                        0.5 * properties.vacuum_length +
                        0.5 * (properties.end_cap_front_length -
                               properties.end_cap_front_thickness -
                               properties.end_cap_window_thickness)),
      air_front_logical, detector_name + "_air_front", end_cap_front_logical, 0,
      0, false);

  /******** Crystals ********/

  G4VSolid *crystal_full_solid = detector_crystal("", 0.);
  vector<G4LogicalVolume *> crystal_full_logical(4);
  const vector<G4Color> colors = {G4Color::Blue(), G4Color::Red(),
                                  G4Color::Green(), G4Color::Brown()};
  const double crystal_distance_from_center_xy =
      22. * mm + 0.5 * properties.crystal_gap;
  const vector<std::pair<double, double>> crystal_xy = {
      {crystal_distance_from_center_xy, crystal_distance_from_center_xy},
      {-crystal_distance_from_center_xy, crystal_distance_from_center_xy},
      {-crystal_distance_from_center_xy, -crystal_distance_from_center_xy},
      {crystal_distance_from_center_xy, -crystal_distance_from_center_xy}};
  const double crystal_z = -0.5 * properties.vacuum_length +
                           0.5 * properties.crystal_length +
                           properties.end_cap_to_crystal_gap_front;
  const vector<double> crystal_rotation = {0. * deg, -90. * deg, -180. * deg,
                                           -270. * deg};
  G4RotationMatrix *crystal_rotation_matrix;

  for (size_t n_crystal = 0; n_crystal < 4; ++n_crystal) {
    crystal_full_logical[n_crystal] = new G4LogicalVolume(
        crystal_full_solid, nist->FindOrBuildMaterial("G4_Ge"),
        detector_name + "_1_logical");
    crystal_full_logical[n_crystal]->SetVisAttributes(
        new G4VisAttributes(colors[n_crystal]));
    crystal_rotation_matrix = new G4RotationMatrix();
    crystal_rotation_matrix->rotateZ(crystal_rotation[n_crystal]);
    new G4PVPlacement(crystal_rotation_matrix,
                      G4ThreeVector(crystal_xy[n_crystal].first,
                                    crystal_xy[n_crystal].second, crystal_z),
                      crystal_full_logical[n_crystal],
                      detector_name + "_crystal_" + to_string(n_crystal + 1),
                      vacuum_logical, 0, 0, false);
  }

  //   /******** Crystals (active volume) ********/

  vector<G4VSolid *> crystal_solid(4);
  for (size_t n_crystal = 0; n_crystal < 4; ++n_crystal) {
    crystal_solid[n_crystal] =
        detector_crystal(to_string(n_crystal + 1), dead_layer[n_crystal]);
    sensitive_logical_volumes.push_back(new G4LogicalVolume(
        crystal_solid[n_crystal], nist->FindOrBuildMaterial("G4_Ge"),
        detector_name + "_" + to_string(n_crystal + 1)));
    sensitive_logical_volumes[n_crystal]->SetVisAttributes(
        new G4VisAttributes(colors[n_crystal]));
    new G4PVPlacement(0, G4ThreeVector(), sensitive_logical_volumes[n_crystal],
                      detector_name + "_crystal_" + to_string(n_crystal + 1) +
                          "_active",
                      crystal_full_logical[n_crystal], 0, 0, false);
  }

  /******** Back end cap *********/

  G4VSolid *end_cap_back_solid =
      rounded_box("end_cap_back_solid", properties.end_cap_back_side_length,
                  properties.end_cap_back_length,
                  properties.end_cap_back_rounding_radius, 20);
  G4LogicalVolume *end_cap_back_logical = new G4LogicalVolume(
      end_cap_back_solid,
      nist->FindOrBuildMaterial(properties.end_cap_material),
      detector_name + "_end_cap_back_logical");
  new G4PVPlacement(rotation,
                    global_coordinates +
                        (dist_from_center + properties.end_cap_front_length +
                         0.5 * properties.end_cap_back_length) *
                            symmetry_axis,
                    end_cap_back_logical, detector_name + "end_cap_back",
                    world_logical, 0, 0, false);

  /******** Air inside the back end cap ********/

  G4VSolid *air_back_solid = rounded_box(
      "_air_back_solid",
      properties.end_cap_back_side_length -
          2. * properties.end_cap_back_thickness,
      properties.end_cap_back_length - 2. * properties.end_cap_back_thickness,
      properties.end_cap_back_rounding_radius, 20);

  G4LogicalVolume *air_back_logical =
      new G4LogicalVolume(air_back_solid, nist->FindOrBuildMaterial("G4_AIR"),
                          detector_name + "_air_back_logical");
  air_back_logical->SetVisAttributes(G4Color::Red());
  new G4PVPlacement(
      0,
      G4ThreeVector(0., 0.,
                    -0.5 * properties.end_cap_back_length +
                        properties.end_cap_back_thickness +
                        0.5 * (properties.end_cap_back_length -
                               2. * properties.end_cap_back_thickness)),
      air_back_logical, detector_name + "_air_back", end_cap_back_logical, 0, 0,
      false);

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
    new G4PVPlacement(rotation,
                      global_coordinates +
                          (dist_from_center + properties.end_cap_front_length +
                           properties.end_cap_back_length +
                           properties.connection_length * 0.5) *
                              symmetry_axis,
                      connection_logical, detector_name + "connection",
                      world_logical, 0, 0, false);

    /************* Dewar *************/
    double dewar_side_length =
        properties.dewar_length - 2. * properties.dewar_wall_thickness;

    // Dewar face
    G4Tubs *dewar_face_solid = new G4Tubs(
        detector_name + "_dewar_face_solid", 0., properties.dewar_outer_radius,
        properties.dewar_wall_thickness * 0.5, 0., twopi);
    G4LogicalVolume *dewar_face_logical = new G4LogicalVolume(
        dewar_face_solid, nist->FindOrBuildMaterial(properties.dewar_material),
        detector_name + "_dewar_face_logical");
    dewar_face_logical->SetVisAttributes(new G4VisAttributes(G4Color::Brown()));
    new G4PVPlacement(rotation,
                      global_coordinates +
                          (dist_from_center + properties.end_cap_front_length +
                           properties.end_cap_back_length +
                           properties.connection_length +
                           properties.dewar_wall_thickness * 0.5) *
                              symmetry_axis,
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
        rotation,
        global_coordinates +
            (dist_from_center + properties.end_cap_front_length +
             properties.end_cap_back_length + properties.connection_length +
             properties.dewar_wall_thickness + dewar_side_length * 0.5) *
                symmetry_axis,
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
    new G4PVPlacement(rotation,
                      global_coordinates +
                          (dist_from_center + properties.end_cap_front_length +
                           properties.end_cap_back_length +
                           properties.connection_length + dewar_side_length +
                           1.5 * properties.dewar_wall_thickness) *
                              symmetry_axis,
                      dewar_base_logical, detector_name + "_dewar_base",
                      world_logical, 0, 0, false);
  }
}

void HPGe_Clover::Construct_Filter_Case(G4LogicalVolume *world_logical,
                                        G4ThreeVector global_coordinates,
                                        double filter_dist_from_center) {

  PLA pla;

  const double filter_case_x = 87 * mm;
  const double filter_case_y = 4.5 * mm;
  const double filter_case_z = 37 * mm;
  const double filter_case_radius = 47 * mm;
  const double filter_case_gap = 7 * mm;
  const double filter_case_top_thickness = 7 * mm;
  const double corner_radius = (filter_case_y + filter_case_radius +
                                filter_case_gap - (0.5 * filter_case_x));
  const G4ThreeVector e_r = unit_vector_r(theta, phi);
  const G4ThreeVector e_t = unit_vector_theta(theta, phi);
  const G4ThreeVector e_p = unit_vector_phi(theta, phi);

  // Filter Case Rectangles

  // North Part of Case
  G4Box *filter_case_side_solid =
      new G4Box(detector_name + "_filter_case_side_solid", 0.5 * filter_case_x,
                0.5 * filter_case_y, 0.5 * filter_case_z);
  G4LogicalVolume *filter_case_north_logical = new G4LogicalVolume(
      filter_case_side_solid, G4Material::GetMaterial("PLA"),
      detector_name + "_filter_case_north_logical");
  filter_case_north_logical->SetVisAttributes(
      new G4VisAttributes(G4Color::Blue()));
  G4RotationMatrix *rotate_north = new G4RotationMatrix();
  rotate_north->rotateZ(90. * deg - phi);
  rotate_north->rotateX(theta);
  rotate_north->rotateZ(intrinsic_rotation_angle);

  new G4PVPlacement(
      rotate_north,
      global_coordinates +
          (filter_dist_from_center + 0.5 * filter_case_z -
           filter_case_top_thickness) *
              e_r +
          (0.5 * filter_case_y + (filter_case_radius + filter_case_gap)) *
              (cos(-intrinsic_rotation_angle) * e_t +
               sin(-intrinsic_rotation_angle) * e_p),
      filter_case_north_logical, detector_name + "_fliter_case_north",
      world_logical, false, 0, false);

  // East Part of Case
  G4LogicalVolume *filter_case_east_logical = new G4LogicalVolume(
      filter_case_side_solid, G4Material::GetMaterial("PLA"),
      detector_name + "_filter_case_east_logical");
  filter_case_east_logical->SetVisAttributes(
      new G4VisAttributes(G4Color::Blue()));
  G4RotationMatrix *rotate_east = new G4RotationMatrix();
  rotate_east->rotateZ(90. * deg - phi);
  rotate_east->rotateX(theta);
  rotate_east->rotateZ(intrinsic_rotation_angle);
  rotate_east->rotateZ(90 * deg);
  new G4PVPlacement(
      rotate_east,
      global_coordinates +
          (filter_dist_from_center + 0.5 * filter_case_z -
           filter_case_top_thickness) *
              e_r +
          (0.5 * filter_case_y + (filter_case_radius + filter_case_gap)) *
              (cos(intrinsic_rotation_angle) * e_p +
               sin(intrinsic_rotation_angle) * e_t),
      filter_case_east_logical, detector_name + "_filter_case_east",
      world_logical, false, 0, false);

  // South Part of Case
  G4LogicalVolume *filter_case_south_logical = new G4LogicalVolume(
      filter_case_side_solid, G4Material::GetMaterial("PLA"),
      detector_name + "_filter_case_south_logical");
  filter_case_south_logical->SetVisAttributes(
      new G4VisAttributes(G4Color::Blue()));
  G4RotationMatrix *rotate_south = new G4RotationMatrix();
  rotate_south->rotateZ(90. * deg - phi);
  rotate_south->rotateX(theta);
  rotate_south->rotateZ(intrinsic_rotation_angle);
  rotate_south->rotateZ(180 * deg);
  new G4PVPlacement(
      rotate_south,
      global_coordinates +
          (filter_dist_from_center + 0.5 * filter_case_z -
           filter_case_top_thickness) *
              e_r +
          (-0.5 * filter_case_y - (filter_case_radius + filter_case_gap)) *
              (cos(-intrinsic_rotation_angle) * e_t +
               sin(-intrinsic_rotation_angle) * e_p),
      filter_case_south_logical, detector_name + "_filter_case_south",
      world_logical, false, 0, false);

  // West Part of Case
  G4LogicalVolume *filter_case_west_logical = new G4LogicalVolume(
      filter_case_side_solid, G4Material::GetMaterial("PLA"),
      detector_name + "_filter_case_west_logical");
  filter_case_west_logical->SetVisAttributes(
      new G4VisAttributes(G4Color::Blue()));
  G4RotationMatrix *rotate_west = new G4RotationMatrix();
  rotate_west->rotateZ(90. * deg - phi);
  rotate_west->rotateX(theta);
  rotate_west->rotateZ(intrinsic_rotation_angle);
  rotate_west->rotateZ(270 * deg);
  new G4PVPlacement(
      rotate_west,
      global_coordinates +
          (filter_dist_from_center + 0.5 * filter_case_z -
           filter_case_top_thickness) *
              e_r +
          (-0.5 * filter_case_y - (filter_case_radius + filter_case_gap)) *
              (cos(intrinsic_rotation_angle) * e_p +
               sin(intrinsic_rotation_angle) * e_t),
      filter_case_west_logical, detector_name + "_filter_case_west",
      world_logical, false, 0, false);

  // Filter Case Curves

  // NE Curve
  G4Tubs *curve_solid =
      new G4Tubs(detector_name + "_curve_solid",
                 filter_case_radius + filter_case_gap - (0.5 * filter_case_x),
                 filter_case_y + filter_case_radius + filter_case_gap -
                     (0.5 * filter_case_x),
                 filter_case_z * 0.5, 0, 90 * deg);
  G4LogicalVolume *curve_NE_logical =
      new G4LogicalVolume(curve_solid, G4Material::GetMaterial("PLA"),
                          detector_name + "_curve_NE_logical");
  curve_NE_logical->SetVisAttributes(new G4VisAttributes(G4Color::Blue()));
  new G4PVPlacement(rotate_west,
                    global_coordinates +
                        (filter_dist_from_center + 0.5 * filter_case_z -
                         filter_case_top_thickness) *
                            e_r +
                        1. / sqrt(2.) * filter_case_x *
                            (cos(45. * deg + intrinsic_rotation_angle) * e_p +
                             sin(45. * deg + intrinsic_rotation_angle) * e_t),
                    curve_NE_logical, detector_name + "_curve_NE",
                    world_logical, false, 0, false);

  // SE Curve
  G4LogicalVolume *curve_SE_logical =
      new G4LogicalVolume(curve_solid, G4Material::GetMaterial("PLA"),
                          detector_name + "_curve_SE_logical");
  curve_SE_logical->SetVisAttributes(new G4VisAttributes(G4Color::Blue()));
  new G4PVPlacement(rotate_south,
                    global_coordinates +
                        (filter_dist_from_center + 0.5 * filter_case_z -
                         filter_case_top_thickness) *
                            e_r +
                        1. / sqrt(2.) * filter_case_x *
                            (cos(45. * deg - intrinsic_rotation_angle) * e_p -
                             sin(45. * deg - intrinsic_rotation_angle) * e_t),
                    curve_SE_logical, detector_name + "_curve_SE",
                    world_logical, false, 0, false);

  // SW Curve
  G4LogicalVolume *curve_SW_logical =
      new G4LogicalVolume(curve_solid, G4Material::GetMaterial("PLA"),
                          detector_name + "_curve_SW_logical");
  curve_SW_logical->SetVisAttributes(new G4VisAttributes(G4Color::Blue()));
  new G4PVPlacement(rotate_east,
                    global_coordinates +
                        (filter_dist_from_center + 0.5 * filter_case_z -
                         filter_case_top_thickness) *
                            e_r -
                        1. / sqrt(2.) * filter_case_x *
                            (cos(45. * deg + intrinsic_rotation_angle) * e_p +
                             sin(45. * deg + intrinsic_rotation_angle) * e_t),
                    curve_SW_logical, detector_name + "_curve_SW",
                    world_logical, false, 0, false);

  // NW Curve
  G4LogicalVolume *curve_NW_logical =
      new G4LogicalVolume(curve_solid, G4Material::GetMaterial("PLA"),
                          detector_name + "_curve_NW_logical");
  curve_NW_logical->SetVisAttributes(new G4VisAttributes(G4Color::Blue()));
  new G4PVPlacement(rotate_north,
                    global_coordinates +
                        (filter_dist_from_center + 0.5 * filter_case_z -
                         filter_case_top_thickness) *
                            e_r +
                        1. / sqrt(2.) * filter_case_x *
                            (-cos(45. * deg - intrinsic_rotation_angle) * e_p +
                             sin(45. * deg - intrinsic_rotation_angle) * e_t),
                    curve_NW_logical, detector_name + "_curve_NW",
                    world_logical, false, 0, false);

  // Flat Box
  G4Box *flat_box = new G4Box(
      "flat_box", (corner_radius + filter_case_x + filter_case_gap) * 0.5,
      (corner_radius + filter_case_x + filter_case_gap) * 0.5,
      0.5 * filter_case_top_thickness);
  G4Tubs *circular_hole =
      new G4Tubs(detector_name + "_circular_hole", 0, filter_case_radius,
                 filter_case_z, 0, twopi);

  // Substitutions
  G4Tubs *curve_solid_sub =
      new G4Tubs(detector_name + "_curve_solid_sub",
                 filter_case_radius + filter_case_gap - (0.5 * filter_case_x),
                 filter_case_y + filter_case_radius + filter_case_gap,
                 filter_case_z * 0.5, 0, 90 * deg);

  rotate_west = new G4RotationMatrix();
  rotate_west->rotateZ(270 * deg);
  G4SubtractionSolid *curve_piece_1 = new G4SubtractionSolid(
      detector_name + "_curve_piece_1", flat_box, curve_solid_sub, rotate_west,
      G4ThreeVector(-0.5 * filter_case_x, 0.5 * filter_case_x, 0));

  G4SubtractionSolid *curve_piece_2 = new G4SubtractionSolid(
      detector_name + "_curve_piece_2", curve_piece_1, curve_solid_sub, 0,
      G4ThreeVector(0.5 * filter_case_x, 0.5 * filter_case_x, 0));

  rotate_south = new G4RotationMatrix();
  rotate_south->rotateZ(180 * deg);
  G4SubtractionSolid *curve_piece_3 = new G4SubtractionSolid(
      detector_name + "_curve_piece_3", curve_piece_2, curve_solid_sub,
      rotate_south,
      G4ThreeVector(-0.5 * filter_case_x, -0.5 * filter_case_x, 0));

  rotate_east = new G4RotationMatrix();
  rotate_east->rotateZ(90 * deg);
  G4SubtractionSolid *curve_piece_4 = new G4SubtractionSolid(
      detector_name + "_curve_piece_4", curve_piece_3, curve_solid_sub,
      rotate_east, G4ThreeVector(0.5 * filter_case_x, -0.5 * filter_case_x, 0));

  G4SubtractionSolid *flat_box_with_hole = new G4SubtractionSolid(
      detector_name + "_flat_box_with_hole", curve_piece_4, circular_hole);

  G4LogicalVolume *final_curve_piece_logical =
      new G4LogicalVolume(flat_box_with_hole, G4Material::GetMaterial("PLA"),
                          detector_name + "_final_curve_piece_logical");
  final_curve_piece_logical->SetVisAttributes(
      new G4VisAttributes(G4Color::Blue()));
  // The part is rotationally symmetric around the original z axis.
  // rotate_north is the only matrix that was not redefined above, so it can be
  // used without loss of generality.
  new G4PVPlacement(
      rotate_north,
      global_coordinates +
          (filter_dist_from_center - 0.5 * filter_case_top_thickness) * e_r,
      final_curve_piece_logical, detector_name + "_final_curve_piece",
      world_logical, false, 0, false);
}

G4VSolid *HPGe_Clover::rounded_box(const string name, const double side_length,
                                   const double length,
                                   const double rounding_radius,
                                   const int n_points_per_corner) {

  const double inverse_n_points_per_corner = 1. / (n_points_per_corner - 1.);
  vector<G4TwoVector> base(4 * n_points_per_corner);

  for (int i = 0; i < n_points_per_corner; ++i) {
    base[i] =
        G4TwoVector(-0.5 * side_length + 0.5 * rounding_radius -
                        0.5 * rounding_radius *
                            sin(0.5 * pi * i * inverse_n_points_per_corner),
                    -0.5 * side_length + 0.5 * rounding_radius -
                        0.5 * rounding_radius *
                            cos(0.5 * pi * i * inverse_n_points_per_corner));
    base[i + n_points_per_corner] =
        G4TwoVector(-0.5 * side_length + 0.5 * rounding_radius -
                        0.5 * rounding_radius *
                            cos(0.5 * pi * i * inverse_n_points_per_corner),
                    0.5 * side_length - 0.5 * rounding_radius +
                        0.5 * rounding_radius *
                            sin(0.5 * pi * i * inverse_n_points_per_corner));
    base[i + 2 * n_points_per_corner] =
        G4TwoVector(0.5 * side_length - 0.5 * rounding_radius +
                        0.5 * rounding_radius *
                            sin(0.5 * pi * i * inverse_n_points_per_corner),
                    0.5 * side_length - 0.5 * rounding_radius +
                        0.5 * rounding_radius *
                            cos(0.5 * pi * i * inverse_n_points_per_corner));
    base[i + 3 * n_points_per_corner] =
        G4TwoVector(0.5 * side_length - 0.5 * rounding_radius +
                        0.5 * rounding_radius *
                            cos(0.5 * pi * i * inverse_n_points_per_corner),
                    -0.5 * side_length + 0.5 * rounding_radius -
                        0.5 * rounding_radius *
                            sin(0.5 * pi * i * inverse_n_points_per_corner));
  }

  return new G4ExtrudedSolid(name, base, length * 0.5, 0., 1., 0., 1.);
}

G4VSolid *HPGe_Clover::detector_crystal(const string suffix,
                                        const double _dead_layer) {
  G4Tubs *crystal_full_solid = new G4Tubs(
      detector_name + "_crystal_full_solid_" + suffix, 0.,
      (1. - _dead_layer) * properties.crystal_radius,
      (1. - _dead_layer) * properties.crystal_length * 0.5, 0., twopi);
  G4Tubs *anode_solid =
      new G4Tubs(detector_name + "_anode_solid_" + suffix, 0.,
                 (1. + _dead_layer) * properties.anode_radius,
                 (1. + _dead_layer) * properties.anode_length * 0.5, 0., twopi);
  G4SubtractionSolid *crystal_original = new G4SubtractionSolid(
      detector_name + "_crystal_original_" + suffix, crystal_full_solid,
      anode_solid, 0,
      G4ThreeVector(0., 0.,
                    0.5 * (1. - _dead_layer) * properties.crystal_length -
                        0.5 * (1. + _dead_layer) * properties.anode_length));
  G4Box *subtraction_solid =
      new G4Box(detector_name + "_subtraction_solid_" + suffix,
                (1. - _dead_layer) * properties.crystal_radius,
                (1. - _dead_layer) * properties.crystal_radius,
                (1. - _dead_layer) * properties.crystal_length);
  G4SubtractionSolid *crystal_step1_solid = new G4SubtractionSolid(
      detector_name + "_crystal_step1_solid_" + suffix, crystal_original,
      subtraction_solid, 0,
      G4ThreeVector((1. - _dead_layer) * (properties.crystal_radius + 23. * mm),
                    0., 0.));
  G4SubtractionSolid *crystal_step2_solid = new G4SubtractionSolid(
      detector_name + "_crystal_step2_solid_" + suffix, crystal_step1_solid,
      subtraction_solid, 0,
      G4ThreeVector((1. - _dead_layer) *
                        (-properties.crystal_radius - 22. * mm),
                    0., 0.));
  G4SubtractionSolid *crystal_step3_solid = new G4SubtractionSolid(
      detector_name + "_crystal_step3_solid_" + suffix, crystal_step2_solid,
      subtraction_solid, 0,
      G4ThreeVector(
          0., (1. - _dead_layer) * (-properties.crystal_radius - 22. * mm),
          0.));
  return new G4SubtractionSolid(
      detector_name + "_crystal_step4_solid_" + suffix, crystal_step3_solid,
      subtraction_solid, 0,
      G4ThreeVector(
          0., (1. - _dead_layer) * (properties.crystal_radius + 23. * mm), 0.));
}

G4VSolid *HPGe_Clover::Filter_Shape(const string name,
                                    const Filter &filter) const {
  return rounded_box(name, filter.radius, filter.thickness,
                     properties.end_cap_front_rounding_radius, 20);
}