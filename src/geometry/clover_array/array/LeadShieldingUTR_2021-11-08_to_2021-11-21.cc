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

#include "G4Box.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

#include "BeamPipe.hh"
#include "LeadShieldingUTR_2021-08-23_to_2021-09-09.hh"

void LeadShieldingUTR::Construct(const G4ThreeVector global_coordinates) {
  const double inch = 25.4 * mm;

  const double gap_size = 1. * mm; // Gap between the inside of the holes in the
                                   // lead walls and the beam pipe, estimated

  const double array_central_wall_to_target =
      10.5 * inch; // Estimated. The central wall protruded by about one brick
                   // width from the side walls in downstream direction.
  const double brick_length = 8. * inch;
  const double brick_width = 4. * inch;
  const double brick_height = 2. * inch;

  const double array_side_wall_to_target =
      14.5 *
      inch; // The side walls touched the horizontal plate of the clover array.
  const double array_side_wall_to_beam_pipe =
      4.75 * inch; // The side walls were mounted on the outside of the holder
                   // of detector B3. The outer dimensions of the holder's base
                   // defined the distance from the beam pipe in x direction.
  const double array_side_wall_thickness = 8. * inch;
  const double array_side_wall_width = 4. * inch;
  const double array_side_wall_height = 10. * inch;

  const double downstream_wall_to_target =
      -5. * inch + 10.5 * inch + 0.5 * inch + 25. * inch;
  const double downstream_wall_thickness = 8. * inch;
  const double downstream_wall_width = 24. * inch;
  const double downstream_wall_height = 20. * inch;
  const double downstream_wall_y = -1.5 * inch;

  const double central_wall_to_target = 42.5 * inch;
  const double central_wall_thickness = 12. * inch;
  const double central_wall_width = downstream_wall_width;
  const double central_wall_height = downstream_wall_height;
  const double central_wall_y = downstream_wall_y;

  const double upstream_wall_to_target = 60. * inch; // Estimated
  const double upstream_wall_thickness = 8. * inch;
  const double upstream_wall_width = 28. * inch;
  const double upstream_wall_height = 24. * inch;

  G4NistManager *nist = G4NistManager::Instance();

  // Upstream wall

  G4Box *_upstream_wall_solid =
      new G4Box("upstream_wall_solid", 0.5 * upstream_wall_width,
                0.5 * upstream_wall_height, 0.5 * upstream_wall_thickness);
  G4SubtractionSolid *upstream_wall_solid = new G4SubtractionSolid(
      "upstream_wall_solid", _upstream_wall_solid,
      new G4Tubs("upstream_wall_hole", 0.,
                 BeamPipe::beam_pipe_outer_radius + gap_size,
                 upstream_wall_thickness, 0.,
                 twopi)); // This wall has a circular hole.
  G4LogicalVolume *upstream_wall_logical = new G4LogicalVolume(
      upstream_wall_solid, nist->FindOrBuildMaterial("G4_Pb"),
      "upstream_wall_logical");
  upstream_wall_logical->SetVisAttributes(G4Color::Green());
  new G4PVPlacement(
      0,
      global_coordinates +
          G4ThreeVector(
              0., 0., -upstream_wall_to_target - 0.5 * upstream_wall_thickness),
      upstream_wall_logical, "upstream_wall", world_logical, false, 0, false);

  // Central wall

  G4Box *_central_wall_solid =
      new G4Box("central_wall_solid", 0.5 * central_wall_width,
                0.5 * central_wall_height, 0.5 * central_wall_thickness);
  G4SubtractionSolid *central_wall_solid = new G4SubtractionSolid(
      "central_wall_solid", _central_wall_solid,
      new G4Box(
          "central_wall_hole", BeamPipe::beam_pipe_outer_radius + gap_size,
          BeamPipe::beam_pipe_outer_radius + gap_size, central_wall_thickness),
      0,
      G4ThreeVector(0., -central_wall_y,
                    0.)); // This wall has a rectangular hole.
  G4LogicalVolume *central_wall_logical = new G4LogicalVolume(
      central_wall_solid, nist->FindOrBuildMaterial("G4_Pb"),
      "central_wall_logical");
  central_wall_logical->SetVisAttributes(G4Color::Green());
  new G4PVPlacement(
      0,
      global_coordinates +
          G4ThreeVector(0., central_wall_y,
                        -central_wall_to_target - 0.5 * central_wall_thickness),
      central_wall_logical, "central_wall", world_logical, false, 0, false);

  // Downstream wall

  G4Box *_downstream_wall_solid =
      new G4Box("downstream_wall_solid", 0.5 * downstream_wall_width,
                0.5 * downstream_wall_height, 0.5 * downstream_wall_thickness);
  G4SubtractionSolid *downstream_wall_solid = new G4SubtractionSolid(
      "downstream_wall_solid", _downstream_wall_solid,
      new G4Box("downstream_wall_hole",
                BeamPipe::beam_pipe_outer_radius + gap_size,
                BeamPipe::beam_pipe_outer_radius + gap_size,
                downstream_wall_thickness),
      0,
      G4ThreeVector(0., -downstream_wall_y,
                    0.)); // This wall has a rectangular hole.
  G4LogicalVolume *downstream_wall_logical = new G4LogicalVolume(
      downstream_wall_solid, nist->FindOrBuildMaterial("G4_Pb"),
      "downstream_wall_logical");
  downstream_wall_logical->SetVisAttributes(G4Color::Green());
  new G4PVPlacement(0,
                    global_coordinates +
                        G4ThreeVector(0., downstream_wall_y,
                                      -downstream_wall_to_target -
                                          0.5 * downstream_wall_thickness),
                    downstream_wall_logical, "downstream_wall", world_logical,
                    false, 0, false);

  // Clover-Array wall
  // The positions 'left' and 'right' are defined for a view from upstream.

  // Central wall. Consists of four standard bricks around the beam pipe.
  G4Box *brick_solid = new G4Box("brick_solid", 0.5 * brick_width,
                                 0.5 * brick_height, 0.5 * brick_length);
  G4LogicalVolume *brick_left_logical = new G4LogicalVolume(
      brick_solid, nist->FindOrBuildMaterial("G4_Pb"), "brick_left_logical");
  brick_left_logical->SetVisAttributes(G4Color::Green());
  G4RotationMatrix *rotate_brick_left_right = new G4RotationMatrix();
  rotate_brick_left_right->rotateZ(90. * deg);
  new G4PVPlacement(
      rotate_brick_left_right,
      global_coordinates +
          G4ThreeVector(0.5 * brick_length - 0.5 * brick_height, 0.,
                        -array_central_wall_to_target - 0.5 * brick_length),
      brick_left_logical, "brick_left", world_logical, false, 0, false);

  G4LogicalVolume *brick_right_logical = new G4LogicalVolume(
      brick_solid, nist->FindOrBuildMaterial("G4_Pb"), "brick_right_logical");
  brick_right_logical->SetVisAttributes(G4Color::Green());
  new G4PVPlacement(
      rotate_brick_left_right,
      global_coordinates +
          G4ThreeVector(-0.5 * brick_length + 0.5 * brick_height, 0.,
                        -array_central_wall_to_target - 0.5 * brick_length),
      brick_right_logical, "brick_right", world_logical, false, 0, false);

  G4LogicalVolume *brick_bottom_logical = new G4LogicalVolume(
      brick_solid, nist->FindOrBuildMaterial("G4_Pb"), "brick_bottom_logical");
  brick_bottom_logical->SetVisAttributes(G4Color::Green());
  G4RotationMatrix *rotate_brick_bottom_top = new G4RotationMatrix();
  rotate_brick_bottom_top->rotateY(90. * deg);
  new G4PVPlacement(
      rotate_brick_bottom_top,
      global_coordinates +
          G4ThreeVector(0., -0.5 * brick_width - 0.5 * brick_height,
                        -array_central_wall_to_target - 0.5 * brick_width),
      brick_bottom_logical, "brick_bottom", world_logical, false, 0, false);

  G4LogicalVolume *brick_top_logical = new G4LogicalVolume(
      brick_solid, nist->FindOrBuildMaterial("G4_Pb"), "brick_top_logical");
  brick_top_logical->SetVisAttributes(G4Color::Green());
  new G4PVPlacement(
      rotate_brick_bottom_top,
      global_coordinates +
          G4ThreeVector(0., 0.5 * brick_width + 0.5 * brick_height,
                        -array_central_wall_to_target - 0.5 * brick_width),
      brick_top_logical, "brick_top", world_logical, false, 0, false);

  // Two identical side walls.
  G4Box *array_side_wall_solid =
      new G4Box("array_side_wall_solid", 0.5 * array_side_wall_width,
                0.5 * array_side_wall_height, 0.5 * array_side_wall_thickness);
  G4LogicalVolume *array_side_wall_left_logical = new G4LogicalVolume(
      array_side_wall_solid, nist->FindOrBuildMaterial("G4_Pb"),
      "array_side_wall_left_logical");
  array_side_wall_left_logical->SetVisAttributes(G4Color::Green());
  new G4PVPlacement(
      0,
      global_coordinates +
          G4ThreeVector(
              array_side_wall_to_beam_pipe + 0.5 * array_side_wall_width, 0.,
              -array_side_wall_to_target - 0.5 * array_side_wall_thickness),
      array_side_wall_left_logical, "array_side_wall_left", world_logical,
      false, 0, false);

  G4LogicalVolume *array_side_wall_right_logical = new G4LogicalVolume(
      array_side_wall_solid, nist->FindOrBuildMaterial("G4_Pb"),
      "array_side_wall_right_logical");
  array_side_wall_right_logical->SetVisAttributes(G4Color::Green());
  new G4PVPlacement(
      0,
      global_coordinates +
          G4ThreeVector(
              -array_side_wall_to_beam_pipe - 0.5 * array_side_wall_width, 0.,
              -array_side_wall_to_target - 0.5 * array_side_wall_thickness),
      array_side_wall_right_logical, "array_side_wall_right", world_logical,
      false, 0, false);
}