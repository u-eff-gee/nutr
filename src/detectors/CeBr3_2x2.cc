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

#include "G4Color.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

#include "CeBr3.hh"
#include "CeBr3_2x2.hh"

CeBr3 cebr3;

void CeBr3_2x2::Construct_Detector(G4LogicalVolume *world_logical,
                                   G4ThreeVector global_coordinates) {

  /*********** Dimensions ***********/
  // Front
  const G4double front_and_pmt_length = 185. * mm;

  const G4double front_case_wall_thickness = 0.4 * mm;
  const G4double front_entrance_window_thickness =
      front_case_wall_thickness; // Technical drawing gives no reason to assume
                                 // it should be different from
                                 // front_wall_thickness, but chose to implement
                                 // an independent variable anyway.
  const G4double front_case_length = 42.5 * mm;
  const G4double front_case_outer_radius = 0.5 * 57. * mm;

  // Crystal
  // The crystal dimensions are given as '51 mm x 51 mm', but this is probably
  // the rounded value of 2 inch x 2 inch. Take the exact value here.
  const G4double crystal_length = 50.8 * mm;
  const G4double crystal_radius = 0.5 * 50.8 * mm;
  const G4double crystal_to_entrance_window =
      front_case_outer_radius - front_case_wall_thickness -
      crystal_radius; // Estimated that the gap between the crystal and the
  // entrance window is as wide as the gap between the crystal and the case on
  // its side.

  // PMT

  const G4double pmt_case_outer_radius = 0.5 * 58.8 * mm;
  const G4double pmt_case_length = front_and_pmt_length - front_case_length;
  const G4double pmt_case_wall_thickness = 0.64 * mm;

  const G4double pmt_outer_radius = crystal_radius;
  const G4double pmt_length =
      front_and_pmt_length - front_entrance_window_thickness -
      crystal_to_entrance_window -
      crystal_length; // Assume it extends all the way to the back of the case.
  const G4double pmt_wall_thickness =
      pmt_case_wall_thickness; // Estimated to be the same as the thickness of
                               // the case wall. Not clear from the drawing,
                               // which leaves out most of the PMT anyway.

  // Connector
  const G4double connector_base_length = 220. * mm - front_and_pmt_length;
  const G4double connector_base_wall_thickness =
      pmt_case_wall_thickness; // Assumption
  const G4double connector_base_outer_radius = pmt_case_outer_radius;
  const G4double signal_connector_radius = 3. * mm; // Estimated
  const G4double signal_connector_length = 8. * mm; // Estimated
  const G4double hv_connector_radius = 3. * mm;     // Estimated
  const G4double hv_connector_length = 8. * mm;     // Estimated

  /*********** Materials ***********/

  G4NistManager *nist = G4NistManager::Instance();
  const G4String front_case_material = "G4_Al";
  const G4String pmt_material = "G4_Al"; // Assumption
  const G4String pmt_case_material =
      "G4_Al"; // Assumption (it is called 'magnetic shield' in the drawing)
  const G4String connector_material =
      "G4_Al"; // Assume that the connectors on top are made of the same
               // material
  const G4String crystal_material = "CeBr3";

  /*********** Orientation in space ***********/

  G4ThreeVector e_r = unit_vector_r(theta, phi);
  G4ThreeVector e_theta = unit_vector_theta(theta, phi);
  G4ThreeVector e_phi = unit_vector_phi(theta, phi);

  rotate(theta, phi, intrinsic_rotation_angle);

  /*********** Front ***********/

  // Entrance window
  G4Tubs *front_entrance_window_solid =
      new G4Tubs(detector_name + "_front_entrance_window_sold", 0.,
                 front_case_outer_radius - front_case_wall_thickness,
                 0.5 * front_entrance_window_thickness, 0., twopi);
  G4LogicalVolume *front_entrance_window_logical =
      new G4LogicalVolume(front_entrance_window_solid,
                          nist->FindOrBuildMaterial(front_case_material),
                          detector_name + "_front_entrance_window_logical");
  front_entrance_window_logical->SetVisAttributes(
      new G4VisAttributes(G4Color::Grey()));
  new G4PVPlacement(
      rotation_matrix,
      global_coordinates +
          (dist_from_center + 0.5 * front_entrance_window_thickness) * e_r,
      front_entrance_window_logical, detector_name + "_front_entrance_window",
      world_logical, 0, 0, false);

  // Case
  G4Tubs *front_case_wall_solid =
      new G4Tubs(detector_name + "_front_case_wall_sold",
                 front_case_outer_radius - front_case_wall_thickness,
                 front_case_outer_radius, 0.5 * front_case_length, 0., twopi);
  G4LogicalVolume *front_case_wall_logical = new G4LogicalVolume(
      front_case_wall_solid, nist->FindOrBuildMaterial(front_case_material),
      detector_name + "_front_case_wall_logical");
  front_case_wall_logical->SetVisAttributes(
      new G4VisAttributes(G4Color::Grey()));
  new G4PVPlacement(rotation_matrix,
                    global_coordinates +
                        (dist_from_center + 0.5 * front_case_length) * e_r,
                    front_case_wall_logical, detector_name + "_front_case_wall",
                    world_logical, 0, 0, false);

  G4Tubs *front_entrance_vacuum_solid =
      new G4Tubs(detector_name + "_front_entrance_vacuum_sold", 0.,
                 crystal_radius, 0.5 * crystal_to_entrance_window, 0., twopi);
  G4LogicalVolume *front_entrance_vacuum_logical = new G4LogicalVolume(
      front_entrance_vacuum_solid, nist->FindOrBuildMaterial("G4_Galactic"),
      detector_name + "_front_entrance_vacuum_logical");
  front_entrance_vacuum_logical->SetVisAttributes(
      new G4VisAttributes(G4Color::White()));
  new G4PVPlacement(
      rotation_matrix,
      global_coordinates + (dist_from_center + front_entrance_window_thickness +
                            0.5 * crystal_to_entrance_window) *
                               e_r,
      front_entrance_vacuum_logical, detector_name + "_front_entrance_vacuum",
      world_logical, 0, 0, false);

  G4Tubs *front_case_vacuum_solid =
      new G4Tubs(detector_name + "_front_case_vacuum_sold", crystal_radius,
                 front_case_outer_radius - front_case_wall_thickness,
                 0.5 * front_case_length, 0., twopi);
  G4LogicalVolume *front_case_vacuum_logical = new G4LogicalVolume(
      front_case_vacuum_solid, nist->FindOrBuildMaterial("G4_Galactic"),
      detector_name + "_front_case_vacuum_logical");
  front_case_vacuum_logical->SetVisAttributes(
      new G4VisAttributes(G4Color::White()));
  new G4PVPlacement(
      rotation_matrix,
      global_coordinates + (dist_from_center + front_entrance_window_thickness +
                            0.5 * front_case_length) *
                               e_r,
      front_case_vacuum_logical, detector_name + "_front_case_vacuum",
      world_logical, 0, 0, false);

  /*********** Crystal ***********/

  G4Tubs *crystal_solid =
      new G4Tubs(detector_name + "_crystal_sold", 0., crystal_radius,
                 0.5 * crystal_length, 0., twopi);
  sensitive_logical_volumes.push_back(new G4LogicalVolume(
      crystal_solid, G4Material::GetMaterial(crystal_material), detector_name));
  sensitive_logical_volumes[0]->SetVisAttributes(
      new G4VisAttributes(G4Color::Green()));
  new G4PVPlacement(rotation_matrix,
                    global_coordinates +
                        (dist_from_center + front_entrance_window_thickness +
                         crystal_to_entrance_window + 0.5 * crystal_length) *
                            e_r,
                    sensitive_logical_volumes[0], detector_name + "_crystal",
                    world_logical, 0, 0, false);

  /*********** PMT ***********/
  // PMT
  G4Tubs *pmt_solid = new G4Tubs(detector_name + "_pmt_sold",
                                 pmt_outer_radius - pmt_wall_thickness,
                                 pmt_outer_radius, 0.5 * pmt_length, 0., twopi);
  G4LogicalVolume *pmt_logical =
      new G4LogicalVolume(pmt_solid, nist->FindOrBuildMaterial(pmt_material),
                          detector_name + "_pmt_logical");
  pmt_logical->SetVisAttributes(new G4VisAttributes(G4Color::Blue()));
  new G4PVPlacement(
      rotation_matrix,
      global_coordinates +
          (dist_from_center + front_entrance_window_thickness +
           crystal_to_entrance_window + crystal_length + 0.5 * pmt_length) *
              e_r,
      pmt_logical, detector_name + "_pmt", world_logical, 0, 0, false);

  G4Tubs *pmt_vacuum_solid = new G4Tubs(detector_name + "_pmt_vacuum_sold", 0.,
                                        pmt_outer_radius - pmt_wall_thickness,
                                        0.5 * pmt_length, 0., twopi);
  G4LogicalVolume *pmt_vacuum_logical = new G4LogicalVolume(
      pmt_vacuum_solid, nist->FindOrBuildMaterial("G4_Galactic"),
      detector_name + "_pmt_vacuum_logical");
  pmt_vacuum_logical->SetVisAttributes(new G4VisAttributes(G4Color::White()));
  new G4PVPlacement(rotation_matrix,
                    global_coordinates +
                        (dist_from_center + front_entrance_window_thickness +
                         crystal_to_entrance_window + crystal_length +
                         0.5 * pmt_length) *
                            e_r,
                    pmt_vacuum_logical, detector_name + "_pmt_vacuum",
                    world_logical, 0, 0, false);

  // PMT case
  G4Tubs *pmt_case_solid =
      new G4Tubs(detector_name + "_pmt_case_sold",
                 pmt_case_outer_radius - pmt_case_wall_thickness,
                 pmt_case_outer_radius, 0.5 * pmt_case_length, 0., twopi);
  G4LogicalVolume *pmt_case_logical = new G4LogicalVolume(
      pmt_case_solid, nist->FindOrBuildMaterial(pmt_case_material),
      detector_name + "_pmt_case_logical");
  pmt_case_logical->SetVisAttributes(
      new G4VisAttributes(G4Color(0.75, 0.75, 0.75)));
  new G4PVPlacement(
      rotation_matrix,
      global_coordinates +
          (dist_from_center + front_case_length + 0.5 * pmt_case_length) * e_r,
      pmt_case_logical, detector_name + "_pmt_case", world_logical, 0, 0,
      false);

  G4Tubs *pmt_case_vacuum_solid =
      new G4Tubs(detector_name + "_pmt_case_vacuum_sold", pmt_outer_radius,
                 pmt_case_outer_radius - pmt_wall_thickness,
                 0.5 * pmt_case_length, 0., twopi);
  G4LogicalVolume *pmt_case_vacuum_logical = new G4LogicalVolume(
      pmt_case_vacuum_solid, nist->FindOrBuildMaterial(pmt_case_material),
      detector_name + "_pmt_case_vacuum_logical");
  pmt_case_vacuum_logical->SetVisAttributes(
      new G4VisAttributes(G4Color::White()));
  new G4PVPlacement(
      rotation_matrix,
      global_coordinates +
          (dist_from_center + front_case_length + 0.5 * pmt_case_length) * e_r,
      pmt_case_vacuum_logical, detector_name + "_pmt_case_vacuum",
      world_logical, 0, 0, false);

  /*********** PMT ***********/
  // Connector base
  G4Tubs *connector_base_solid = new G4Tubs(
      detector_name + "_connector_base_sold",
      connector_base_outer_radius - connector_base_wall_thickness,
      connector_base_outer_radius, 0.5 * connector_base_length, 0., twopi);
  G4LogicalVolume *connector_base_logical = new G4LogicalVolume(
      connector_base_solid, nist->FindOrBuildMaterial(connector_material),
      detector_name + "_connector_base_logical");
  connector_base_logical->SetVisAttributes(
      new G4VisAttributes(G4Color::Grey()));
  new G4PVPlacement(rotation_matrix,
                    global_coordinates +
                        (dist_from_center + front_and_pmt_length +
                         0.5 * connector_base_length) *
                            e_r,
                    connector_base_logical, detector_name + "_connector_base",
                    world_logical, 0, 0, false);

  G4Tubs *connector_base_inside_solid = new G4Tubs(
      detector_name + "_connector_base_inside_sold", 0.,
      connector_base_outer_radius - connector_base_wall_thickness,
      0.5 * (connector_base_length - connector_base_wall_thickness), 0., twopi);
  G4LogicalVolume *connector_base_inside_logical = new G4LogicalVolume(
      connector_base_inside_solid, nist->FindOrBuildMaterial("G4_AIR"),
      detector_name +
          "_connector_base_inside_logical"); // Assume it is filled
                                             // predominantly with low-density
                                             // material.
  connector_base_inside_logical->SetVisAttributes(
      new G4VisAttributes(G4Color::White()));
  new G4PVPlacement(
      rotation_matrix,
      global_coordinates +
          (dist_from_center + front_and_pmt_length +
           0.5 * (connector_base_length - connector_base_wall_thickness)) *
              e_r,
      connector_base_inside_logical, detector_name + "_connector_base_inside",
      world_logical, 0, 0, false);

  // Connector lid
  G4Tubs *connector_lid_solid =
      new G4Tubs(detector_name + "_connector_lid_sold", 0.,
                 connector_base_outer_radius - connector_base_wall_thickness,
                 0.5 * connector_base_wall_thickness, 0., twopi);
  G4LogicalVolume *connector_lid_logical = new G4LogicalVolume(
      connector_lid_solid, nist->FindOrBuildMaterial(connector_material),
      detector_name + "_connector_lid_logical");
  connector_lid_logical->SetVisAttributes(new G4VisAttributes(G4Color::Grey()));
  new G4PVPlacement(rotation_matrix,
                    global_coordinates +
                        (dist_from_center + front_and_pmt_length +
                         connector_base_length -
                         0.5 * connector_base_wall_thickness) *
                            e_r,
                    connector_lid_logical, detector_name + "_connector_lid",
                    world_logical, 0, 0, false);

  // HV connector
  G4Tubs *hv_connector_solid =
      new G4Tubs(detector_name + "_hv_connector_sold", 0., hv_connector_radius,
                 0.5 * hv_connector_length, 0., twopi);
  G4LogicalVolume *hv_connector_logical = new G4LogicalVolume(
      hv_connector_solid, nist->FindOrBuildMaterial(connector_material),
      detector_name + "_hv_connector_logical");
  hv_connector_logical->SetVisAttributes(new G4VisAttributes(G4Color::Grey()));
  new G4PVPlacement(rotation_matrix,
                    global_coordinates +
                        (dist_from_center + front_and_pmt_length +
                         connector_base_length + 0.5 * hv_connector_length) *
                            e_r +
                        0.5 / sqrt(2.) * connector_base_outer_radius * e_theta +
                        0.5 / sqrt(2.) * connector_base_outer_radius * e_phi,
                    hv_connector_logical, detector_name + "_hv_connector",
                    world_logical, 0, 0, false);

  // Signal connector
  G4Tubs *signal_connector_solid = new G4Tubs(
      detector_name + "_signal_connector_sold", 0., signal_connector_radius,
      0.5 * signal_connector_length, 0., twopi);
  G4LogicalVolume *signal_connector_logical = new G4LogicalVolume(
      signal_connector_solid, nist->FindOrBuildMaterial(connector_material),
      detector_name + "_signal_connector_logical");
  signal_connector_logical->SetVisAttributes(
      new G4VisAttributes(G4Color::Grey()));
  new G4PVPlacement(
      rotation_matrix,
      global_coordinates +
          (dist_from_center + front_and_pmt_length + connector_base_length +
           0.5 * signal_connector_length) *
              e_r +
          0.5 / sqrt(2.) * connector_base_outer_radius * e_theta -
          0.5 / sqrt(2.) * connector_base_outer_radius * e_phi,
      signal_connector_logical, detector_name + "_signal_connector",
      world_logical, 0, 0, false);
}

void CeBr3_2x2::Construct_Filter_Case(G4LogicalVolume *world_logical,
                                      G4ThreeVector global_coordinates,
                                      double filter_dist_from_center) {

  G4NistManager *nist = G4NistManager::Instance();

  const G4ThreeVector e_r = unit_vector_r(theta, phi);

  const double filter_case_inner_radius = 0.5 * 54.0 * mm;
  const double filter_case_wall_thickness = 4. * mm;
  const double filter_case_length = 38. * mm;
  const double filter_case_front_inner_radius = 0.5 * 51. * mm;
  const double filter_case_front_length = 3. * mm;

  G4Tubs *filter_case_front_solid =
      new G4Tubs(detector_name + "_filter_case_front_solid",
                 filter_case_front_inner_radius,
                 filter_case_inner_radius + filter_case_wall_thickness,
                 filter_case_front_length * 0.5, 0., twopi);
  G4LogicalVolume *filter_case_front_logical = new G4LogicalVolume(
      filter_case_front_solid, nist->FindOrBuildMaterial("G4_PLEXIGLASS"),
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
  G4LogicalVolume *filter_case_logical = new G4LogicalVolume(
      filter_case_solid, nist->FindOrBuildMaterial("G4_PLEXIGLASS"),
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

G4VSolid *CeBr3_2x2::Filter_Shape(const string name,
                                  const Filter &filter) const {
  return new G4Tubs(name, 0., filter.radius, filter.thickness * 0.5, 0., twopi);
}