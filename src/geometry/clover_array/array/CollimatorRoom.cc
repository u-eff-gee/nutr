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
#include "CollimatorRoom.hh"

void CollimatorRoom::Construct(const G4ThreeVector global_coordinates) {
  const double inch = 25.4 * mm;

  const double downstream_wall_to_target = 72. * inch; // Estimated
  const double downstream_wall_to_upstream_wall = 320. * mm;
  const double collimator_to_upstream_wall = 200. * mm;
  const double precollimator_to_collimator = 500. * mm;

  const double gap_size = 1. * mm; // Gap between the inside of the holes in the
                                   // lead walls and the beam pipe, estimated

  const double precollimator_length = 9. * inch;
  const double precollimator_outer_radius = 2. * inch;
  const double precollimator_inner_radius = 0.75 * inch;

  const double collimator_width = 60. * mm;
  const double collimator_height = 60. * mm;

  const double upstream_wall_thickness = 16. * inch;
  const double upstream_wall_width = 24. * inch;
  const double upstream_wall_height = 16. * inch;

  const double collimator_room_wall_thickness = 4. * inch;
  const double collimator_room_wall_width =
      2000. * mm; // Estimated, width and height should be large enough to take
                  // into account the effect of the wall on the HIγS beam.
  const double collimator_room_wall_height = 2000. * mm;     // Estimated
  const double collimator_room_wall_hole_radius = 2. * inch; // Estimated

  const double downstream_wall_thickness = 16. * inch;
  const double downstream_wall_width = 24. * inch;
  const double downstream_wall_height = 16. * inch;

  G4NistManager *nist = G4NistManager::Instance();

  // Precollimator

  G4Tubs *precollimator_solid = new G4Tubs(
      "precollimator_solid", precollimator_inner_radius,
      precollimator_outer_radius, 0.5 * precollimator_length, 0., twopi);
  G4LogicalVolume *precollimator_logical = new G4LogicalVolume(
      precollimator_solid, nist->FindOrBuildMaterial("G4_Cu"),
      "precollimator_logical");
  precollimator_logical->SetVisAttributes(G4Color::Red());
  new G4PVPlacement(
      0,
      global_coordinates +
          G4ThreeVector(
              0., 0.,
              -downstream_wall_to_target - downstream_wall_thickness -
                  downstream_wall_to_upstream_wall - upstream_wall_thickness -
                  collimator_to_upstream_wall - collimator_length -
                  precollimator_to_collimator - 0.5 * precollimator_length),
      precollimator_logical, "precollimator", world_logical, false, 0, false);

  // Collimator

  G4Box *_collimator_solid =
      new G4Box("collimator_solid", 0.5 * collimator_width,
                0.5 * collimator_height, 0.5 * collimator_length);
  G4SubtractionSolid *collimator_solid = new G4SubtractionSolid(
      "collimator_solid", _collimator_solid,
      new G4Tubs("collimator_hole", 0., collimator_radius, collimator_length,
                 0., twopi));
  G4LogicalVolume *collimator_logical = new G4LogicalVolume(
      collimator_solid, nist->FindOrBuildMaterial(collimator_material),
      "collimator_logical");
  collimator_logical->SetVisAttributes(G4Color::White());
  new G4PVPlacement(
      0,
      global_coordinates +
          G4ThreeVector(
              0., 0.,
              -downstream_wall_to_target - downstream_wall_thickness -
                  downstream_wall_to_upstream_wall - upstream_wall_thickness -
                  collimator_to_upstream_wall - 0.5 * collimator_length),
      collimator_logical, "collimator", world_logical, false, 0, false);

  // Upstream wall

  G4Box *_collimator_room_upstream_wall_solid =
      new G4Box("upstream_wall_solid", 0.5 * upstream_wall_width,
                0.5 * upstream_wall_height, 0.5 * upstream_wall_thickness);
  G4SubtractionSolid *collimator_room_upstream_wall_solid =
      new G4SubtractionSolid(
          "collimator_room_upstream_wall_solid",
          _collimator_room_upstream_wall_solid,
          new G4Tubs("collimator_room_upstream_wall_hole", 0.,
                     BeamPipe::beam_pipe_outer_radius + gap_size,
                     upstream_wall_thickness, 0., twopi));
  G4LogicalVolume *collimator_room_upstream_wall_logical = new G4LogicalVolume(
      collimator_room_upstream_wall_solid, nist->FindOrBuildMaterial("G4_Pb"),
      "collimator_room_upstream_wall_logical");
  collimator_room_upstream_wall_logical->SetVisAttributes(G4Color::Green());
  new G4PVPlacement(
      0,
      global_coordinates +
          G4ThreeVector(0., 0.,
                        -downstream_wall_to_target - downstream_wall_thickness -
                            downstream_wall_to_upstream_wall -
                            0.5 * upstream_wall_thickness),
      collimator_room_upstream_wall_logical, "collimator_room_upstream_wall",
      world_logical, false, 0, false);

  // Wall between collimator room and UTR

  G4Box *collimator_room_wall_without_hole_solid = new G4Box(
      "collimator_room_wall_without_hole_solid",
      0.5 * collimator_room_wall_width, 0.5 * collimator_room_wall_height,
      0.5 * collimator_room_wall_thickness);
  G4SubtractionSolid *collimator_room_wall_solid = new G4SubtractionSolid(
      "collimator_room_wall_solid", collimator_room_wall_without_hole_solid,
      new G4Tubs("collimator_room_wall_hole", 0.,
                 collimator_room_wall_hole_radius,
                 collimator_room_wall_thickness, 0., twopi));
  G4LogicalVolume *collimator_room_wall_logical = new G4LogicalVolume(
      collimator_room_wall_solid, nist->FindOrBuildMaterial("G4_CONCRETE"),
      "collimator_room_wall_logical");
  collimator_room_wall_logical->SetVisAttributes(G4Color::White());
  new G4PVPlacement(0,
                    global_coordinates +
                        G4ThreeVector(0., 0.,
                                      -downstream_wall_to_target +
                                          0.5 * collimator_room_wall_thickness),
                    collimator_room_wall_logical, "collimator_room_wall",
                    world_logical, false, 0, false);

  // Downstream wall

  G4Box *_collimator_room_downstream_wall_solid =
      new G4Box("downstream_wall_solid", 0.5 * downstream_wall_width,
                0.5 * downstream_wall_height, 0.5 * downstream_wall_thickness);
  G4SubtractionSolid *collimator_room_downstream_wall_solid =
      new G4SubtractionSolid(
          "collimator_room_downstream_wall_solid",
          _collimator_room_downstream_wall_solid,
          new G4Tubs("collimator_room_downstream_wall_hole", 0.,
                     BeamPipe::beam_pipe_outer_radius + gap_size,
                     downstream_wall_thickness, 0., twopi));
  G4LogicalVolume *collimator_room_downstream_wall_logical =
      new G4LogicalVolume(collimator_room_downstream_wall_solid,
                          nist->FindOrBuildMaterial("G4_Pb"),
                          "collimator_room_downstream_wall_logical");
  collimator_room_downstream_wall_logical->SetVisAttributes(G4Color::Green());
  new G4PVPlacement(
      0,
      global_coordinates + G4ThreeVector(0., 0.,
                                         -downstream_wall_to_target -
                                             0.5 * downstream_wall_thickness),
      collimator_room_downstream_wall_logical,
      "collimator_room_downstream_wall", world_logical, false, 0, false);
}