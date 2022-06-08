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
#include "LeadShieldingUTR_2021-02-16_to_2021-05-06.hh"

void LeadShieldingUTR::Construct(const G4ThreeVector global_coordinates) {
  const double inch = 25.4 * mm;

  const double gap_size = 1. * mm; // Gap between the inside of the holes in the
                                   // lead walls and the beam pipe, estimated

  const double downstream_wall_to_target = 46.5 * inch;
  const double downstream_wall_thickness = 8. * inch;
  const double downstream_wall_width = 24. * inch;
  const double downstream_wall_height = 20. * inch;

  const double upstream_wall_to_target = 60. * inch; // Estimated
  const double upstream_wall_thickness = 8. * inch;
  const double upstream_wall_width = 28. * inch;
  const double upstream_wall_height = 24. * inch;

  const double wrap_thickness = 3. * mm;
  const double wrap_length = downstream_wall_to_target - 7. * inch;

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

  // Downstream wall

  G4Box *_downstream_wall_solid =
      new G4Box("downstream_wall_solid", 0.5 * downstream_wall_width,
                0.5 * downstream_wall_height, 0.5 * downstream_wall_thickness);
  G4SubtractionSolid *downstream_wall_solid = new G4SubtractionSolid(
      "downstream_wall_solid", _downstream_wall_solid,
      new G4Box(
          "downstream_wall_hole", BeamPipe::beam_pipe_outer_radius + gap_size,
          BeamPipe::beam_pipe_outer_radius + gap_size,
          downstream_wall_thickness)); // This wall has a rectangular hole.
  G4LogicalVolume *downstream_wall_logical = new G4LogicalVolume(
      downstream_wall_solid, nist->FindOrBuildMaterial("G4_Pb"),
      "downstream_wall_logical");
  downstream_wall_logical->SetVisAttributes(G4Color::Green());
  new G4PVPlacement(0,
                    global_coordinates +
                        G4ThreeVector(0., 0.,
                                      -downstream_wall_to_target -
                                          0.5 * downstream_wall_thickness),
                    downstream_wall_logical, "downstream_wall", world_logical,
                    false, 0, false);

  // Wrapping

  G4Tubs *wrap_solid =
      new G4Tubs("wrap_solid", BeamPipe::beam_pipe_outer_radius,
                 BeamPipe::beam_pipe_outer_radius + wrap_thickness,
                 0.5 * wrap_length, 0., twopi);
  G4LogicalVolume *wrap_logical = new G4LogicalVolume(
      wrap_solid, nist->FindOrBuildMaterial("G4_Pb"), "wrap_logical");
  wrap_logical->SetVisAttributes(G4Color::Green());
  new G4PVPlacement(
      0,
      global_coordinates +
          G4ThreeVector(0., 0., -downstream_wall_to_target + 0.5 * wrap_length),
      wrap_logical, "wrap", world_logical, false, 0, false);
}