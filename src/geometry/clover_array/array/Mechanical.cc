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

#include <vector>

using std::vector;

#include "G4Box.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4Polyhedra.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

#include "Mechanical.hh"

void Mechanical::Construct(const G4ThreeVector global_coordinates) {

  G4NistManager *nist = G4NistManager::Instance();
  const double inch = 25.4 *mm; // Many dimensions are given in inches.
  G4RotationMatrix *rotate_polygon = new G4RotationMatrix(); // Required for rotating the detector window, and the strange plate.
  rotate_polygon->rotateZ(22.5 * deg);

  const double plate_thickness = 0.5*inch;

  // Detector window
  // The detector window is a large vertical octagon with a circular cutout.
  // The holders of all 90-degree-polar-angle detectors are attached to this frame.
  const double detector_window_outer_radius = 40. * inch;
  double zPlane []     = {0, plate_thickness};
  const vector<double> inner(8, 0.);
  const vector<double> outer(8, detector_window_outer_radius);
  const double detector_window_z = 5.*inch; // Orthogonal distance from target position to upstream plane of detector window.
  
  G4Polyhedra* window_plate                  = new G4Polyhedra("window_plate", 0, twopi, 8, 2, zPlane, inner.data(), outer.data());
  G4Tubs* window                             = new G4Tubs("window", 0, 45.72*cm , 5*cm * 0.5, 0, twopi);
  
  G4SubtractionSolid* window_cutout          = new G4SubtractionSolid("window_cutout", window_plate, window);
  
  G4LogicalVolume* detector_window_logical   = new G4LogicalVolume(window_cutout, nist->FindOrBuildMaterial("G4_Al"), "detector_window_logical");
  detector_window_logical -> SetVisAttributes(new G4VisAttributes(G4Color::Gray()));
  new G4PVPlacement(rotate_polygon, global_coordinates + G4ThreeVector(0, 0, detector_window_z), detector_window_logical, "window_cutout", world_logical, false, 0, false);

  // Strange plate
  // The vertical strange plate looks a bit like a flattened wristwatch and holds the 
  // backward-angle CeBr detectors.
  // The "watch display" is an octagon with a circular cutout, basically a smaller version of the 
  // "Detector window", the "watch straps" are two long plates that extend as far to the left and
  // right as the "Detector window".
  const double zPlane2[] = {0, plate_thickness};
  const vector<double> inner2(8, 0.);
  const double strange_plate_outer_radius = 17.5 * inch;
  const vector<double> outer2(8, strange_plate_outer_radius);
  const double strange_plate_to_detector_window = 10.5*inch;

  G4Polyhedra *strange_plate =
      new G4Polyhedra("strange_plate", 0, twopi, 8, 2, zPlane2, inner2.data(), outer2.data());
  const double strange_window_radius = 13. * inch;
  G4Tubs *strange_window =
      new G4Tubs("strange_window", 0, strange_window_radius, 5 * cm * 0.5, 0, twopi);
  const double strange_box_width = detector_window_outer_radius - strange_plate_outer_radius;
  const double strange_box_height = 10.75 * inch;
  G4Box *strange_box = new G4Box("strange_box_one", strange_box_width * 0.5,
                                     strange_box_height * 0.5, plate_thickness * 0.5);

  G4SubtractionSolid *strange_cut_one =
      new G4SubtractionSolid("strange_cut_one", strange_plate, strange_window);

  G4LogicalVolume *strange_plate_logical =
      new G4LogicalVolume(strange_cut_one, nist->FindOrBuildMaterial("G4_Al"),
                          "strange_plate_logical");
  strange_plate_logical->SetVisAttributes(new G4VisAttributes(G4Color::Gray()));

  G4LogicalVolume *strange_side_one_logical =
      new G4LogicalVolume(strange_box, nist->FindOrBuildMaterial("G4_Al"),
                          "strange_side_one_logical");
  strange_side_one_logical->SetVisAttributes(
      new G4VisAttributes(G4Color::Gray()));

  G4LogicalVolume *strange_side_two_logical =
      new G4LogicalVolume(strange_box, nist->FindOrBuildMaterial("G4_Al"),
                          "strange_side_two_logical");
  strange_side_two_logical->SetVisAttributes(
      new G4VisAttributes(G4Color::Gray()));

  const double strange_box_x = strange_plate_outer_radius + 0.5*strange_box_width;

  new G4PVPlacement(
      rotate_polygon,
      global_coordinates + G4ThreeVector(0, 0, detector_window_z-strange_plate_to_detector_window-plate_thickness),
      strange_plate_logical, "strange_cut_one", world_logical, false, 0, false);
  new G4PVPlacement(
      0, global_coordinates + G4ThreeVector(strange_box_x, 0., detector_window_z-strange_plate_to_detector_window-0.5*plate_thickness),
      strange_side_one_logical, "strange_box_one", world_logical, false, 0,
      false);
  new G4PVPlacement(
      0, global_coordinates + G4ThreeVector(-strange_box_x, 0., detector_window_z-strange_plate_to_detector_window-0.5*plate_thickness),
      strange_side_two_logical, "strange_box_two", world_logical, false, 0,
      false);

  // Bridge plate
  // The bridge plate is a horizontal plate with a cutout that has the shape of a half box with 
  // rounded edges.
  // The rounding radius is so large that the cutout almost looks like a semicircle.
  // The holders of all backward-angle clover detectors are attached to the bridge plate.
  //
  // The construction is a little more complicated than need be.
  // All parts are constructed as vertical plates, and later the final part is rotated into the horizontal plane.
  const double box_cutout_rounding_radius = 18.*inch;
  const double box_cutout_straight_side_length = 8.*inch;
  const double bridge_plate_z = 25.*inch;
  const double cutout_depth = 26.*inch;
  G4RotationMatrix* rotate_quarter_one       = new G4RotationMatrix(); rotate_quarter_one -> rotateX(90*deg);
  G4Box* bridge_plate                        = new G4Box("bridge_plate", 60.*inch * 0.5, bridge_plate_z * 0.5, plate_thickness * 0.5);
  G4Box* box_cutout                          = new G4Box("box_cutout", 20.5*cm * 0.5, box_cutout_rounding_radius, plate_thickness);
  G4Tubs* quarter_piece                      = new G4Tubs("quarter_piece", 0, box_cutout_rounding_radius, plate_thickness, 0, pi * 0.5);
  G4Tubs* quarter_piece_two                  = new G4Tubs("quarter_piece_two", 0, box_cutout_rounding_radius, plate_thickness, 270*deg, 360*deg);
  
  G4SubtractionSolid* cutout_quarter_one     = new G4SubtractionSolid("cutout_quarter_one", bridge_plate, quarter_piece, 0, G4ThreeVector(box_cutout_straight_side_length * 0.5, cutout_depth * -0.5, 0));
  G4SubtractionSolid* cutout_rectangle_mid   = new G4SubtractionSolid("cutout_rectangle_mid", cutout_quarter_one, box_cutout, 0, G4ThreeVector(0, cutout_depth * -0.5, 0));
  G4SubtractionSolid* final_bridge           = new G4SubtractionSolid("final_bridge", cutout_rectangle_mid, quarter_piece_two, 0, G4ThreeVector(box_cutout_straight_side_length * -0.5, cutout_depth * -0.5, 0));
  
  G4LogicalVolume* bridge_plate_logical      = new G4LogicalVolume(final_bridge, nist->FindOrBuildMaterial("G4_Al"), "bridge_plate_logical");
  bridge_plate_logical -> SetVisAttributes(new G4VisAttributes(G4Color::Gray()));
  new G4PVPlacement(rotate_quarter_one, global_coordinates + G4ThreeVector(0, -0.5*strange_box_height + 0.5*plate_thickness, detector_window_z - strange_plate_to_detector_window - plate_thickness - 0.5*bridge_plate_z), bridge_plate_logical, "final_bridge", world_logical, false, 0, false);

  // Base plate
  const double base_plate_x = 2.*detector_window_outer_radius;
  const double base_plate_to_detector_window = 1.5*inch; // Top of base plate to bottom of detector window.
  const double base_plate_z = 48.*inch;
  
  G4Box* base_plate_solid = new G4Box("base_plate_solid", 0.5*base_plate_x, 0.5*plate_thickness, 0.5*base_plate_z);
  G4LogicalVolume *base_plate_logical =
      new G4LogicalVolume(base_plate_solid, nist->FindOrBuildMaterial("G4_Al"),
                          "base_plate_logical");
  base_plate_logical->SetVisAttributes(
      new G4VisAttributes(G4Color::Gray()));

  new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., -detector_window_outer_radius-base_plate_to_detector_window-0.5*plate_thickness, detector_window_z - strange_plate_to_detector_window - plate_thickness - bridge_plate_z + 0.5*base_plate_z), base_plate_logical, "base_plate", world_logical, false, 0, false);
}