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

  const double array_wall_to_target =
      13. * inch; // Estimated. CeBr BK had to be pushed a little forward to
                  // make room for this wall. Its back part almost touched the
                  // base of the wall (which is not implemented at the moment).
                  // The wall's position should be fine within +- 0.5 inch.
  const double array_wall_thickness = 8. * inch;
  const double array_wall_width = 16. * inch;
  const double array_wall_height = 10. * inch;

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
      0, G4ThreeVector(0., -downstream_wall_y, 0.));
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
  G4Box *_array_wall_solid =
      new G4Box("array_wall_solid", 0.5 * array_wall_width,
                0.5 * array_wall_height, 0.5 * array_wall_thickness);
  G4SubtractionSolid *array_wall_solid = new G4SubtractionSolid(
      "array_wall_solid", _array_wall_solid,
      new G4Box("array_wall_hole", BeamPipe::beam_pipe_outer_radius + gap_size,
                BeamPipe::beam_pipe_outer_radius + gap_size,
                array_wall_thickness)); // This wall has a rectangular hole.
  G4LogicalVolume *array_wall_logical =
      new G4LogicalVolume(array_wall_solid, nist->FindOrBuildMaterial("G4_Pb"),
                          "array_wall_logical");
  array_wall_logical->SetVisAttributes(G4Color::Green());
  new G4PVPlacement(
      0,
      global_coordinates +
          G4ThreeVector(0., 0.,
                        -array_wall_to_target - 0.5 * array_wall_thickness),
      array_wall_logical, "array_wall", world_logical, false, 0, false);
}