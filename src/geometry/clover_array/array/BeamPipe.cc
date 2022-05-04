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

#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

#include "BeamPipe.hh"

void BeamPipe::Construct(const G4ThreeVector global_coordinates) {

  const double inch = 25.4 * mm;

  const double beam_pipe_length = 2.9 * m; // Estimated
  const double beam_pipe_inner_radius = 0.875 * inch;

  const double insert_pipe_lid_thickness = 2. * mm;        // Estimated
  const double insert_pipe_lid_outer_radius = 1.08 * inch; // Estimated
  const double insert_pipe_inner_radius = 0.8 * inch;
  const double insert_pipe_front_inner_radius = 0.85 * inch; // Estimated
  const double beam_pipe_downstream_lid_to_target_position = 23.75 * inch;
  const double insert_pipe_front_length = 2. * inch; // Estimated
  const double insert_pipe_main_length =
      beam_pipe_downstream_lid_to_target_position - insert_pipe_lid_thickness -
      0.5 * insert_pipe_front_length;

  const double target_ring_length = 1. * inch; // Estimated

  const double upstream_cap_lid_thickness = 2. * mm;
  const double upstream_cap_outer_radius = 1.15 * inch;
  const double upstream_cap_length = 1. * inch;

  G4NistManager *nist = G4NistManager::Instance();

  // Main pipe

  G4double beam_pipe_z = -0.5 * beam_pipe_length +
                         beam_pipe_downstream_lid_to_target_position -
                         insert_pipe_lid_thickness;
  G4Tubs *beam_pipe_solid =
      new G4Tubs("beam_pipe_solid", beam_pipe_inner_radius,
                 beam_pipe_outer_radius, 0.5 * beam_pipe_length, 0., twopi);
  G4LogicalVolume *beam_pipe_logical = new G4LogicalVolume(
      beam_pipe_solid, nist->FindOrBuildMaterial("G4_PLEXIGLASS"),
      "beam_pipe_logical");
  new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 0., beam_pipe_z),
                    beam_pipe_logical, "beam_pipe", world_logical, false, 0,
                    false);

  G4Tubs *beam_pipe_vacuum_solid =
      new G4Tubs("beam_pipe_vacuum_solid", 0., beam_pipe_inner_radius,
                 0.5 * beam_pipe_length, 0., twopi);
  G4LogicalVolume *beam_pipe_vacuum_logical = new G4LogicalVolume(
      beam_pipe_vacuum_solid,
      nist->ConstructNewGasMaterial("beam_pipe_vacuum", "G4_AIR", 293. * kelvin,
                                    0.04 * atmosphere),
      "beam_pipe_vacuum_logical");
  beam_pipe_vacuum_logical->SetVisAttributes(G4Color::Cyan());
  new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 0., beam_pipe_z),
                    beam_pipe_vacuum_logical, "beam_pipe_vacuum", world_logical,
                    false, 0, false);

  // Upstream cap
  if (use_upstream_cap) {
    G4Tubs *upstream_cap_solid = new G4Tubs(
        "upstream_cap_solid", beam_pipe_outer_radius, upstream_cap_outer_radius,
        0.5 * upstream_cap_length, 0., twopi);
    G4LogicalVolume *upstream_cap_logical = new G4LogicalVolume(
        upstream_cap_solid, nist->FindOrBuildMaterial("G4_PLEXIGLASS"),
        "upstream_cap_logical");
    new G4PVPlacement(0,
                      global_coordinates +
                          G4ThreeVector(0., 0.,
                                        beam_pipe_z - 0.5 * beam_pipe_length +
                                            0.5 * upstream_cap_length),
                      upstream_cap_logical, "upstream_cap", world_logical,
                      false, 0, false);

    G4Tubs *upstream_cap_lid_solid =
        new G4Tubs("upstream_cap_lid_solid", 0., upstream_cap_outer_radius,
                   0.5 * upstream_cap_lid_thickness, 0., twopi);
    G4LogicalVolume *upstream_cap_lid_logical = new G4LogicalVolume(
        upstream_cap_lid_solid, nist->FindOrBuildMaterial("G4_PLEXIGLASS"),
        "upstream_cap_lid_logical");
    new G4PVPlacement(0,
                      global_coordinates +
                          G4ThreeVector(0., 0.,
                                        beam_pipe_z - 0.5 * beam_pipe_length -
                                            0.5 * upstream_cap_lid_thickness),
                      upstream_cap_lid_logical, "upstream_cap_lid",
                      world_logical, false, 0, false);
  }

  // Insert pipe
  if (use_downstream_pipe) {
    G4Tubs *insert_pipe_solid = new G4Tubs(
        "insert_pipe_solid", insert_pipe_inner_radius, beam_pipe_inner_radius,
        0.5 * insert_pipe_main_length, 0., twopi);
    G4LogicalVolume *insert_pipe_logical = new G4LogicalVolume(
        insert_pipe_solid, nist->FindOrBuildMaterial("G4_PLEXIGLASS"),
        "insert_pipe_logical");
    new G4PVPlacement(0,
                      global_coordinates +
                          G4ThreeVector(0., 0.,
                                        0.5 * beam_pipe_length -
                                            0.5 * insert_pipe_main_length),
                      insert_pipe_logical, "insert_pipe",
                      beam_pipe_vacuum_logical, false, 0, false);

    G4Tubs *insert_pipe_front_solid = new G4Tubs(
        "insert_pipe_front_solid", insert_pipe_front_inner_radius,
        beam_pipe_inner_radius, 0.5 * insert_pipe_front_length, 0., twopi);
    G4LogicalVolume *insert_pipe_front_logical = new G4LogicalVolume(
        insert_pipe_front_solid, nist->FindOrBuildMaterial("G4_PLEXIGLASS"),
        "insert_pipe_front_logical");
    new G4PVPlacement(0,
                      global_coordinates +
                          G4ThreeVector(0., 0.,
                                        0.5 * beam_pipe_length -
                                            insert_pipe_main_length -
                                            0.5 * insert_pipe_front_length),
                      insert_pipe_front_logical, "insert_pipe_front",
                      beam_pipe_vacuum_logical, false, 0, false);

    G4Tubs *insert_pipe_lid_solid =
        new G4Tubs("insert_pipe_lid_solid", 0., insert_pipe_lid_outer_radius,
                   0.5 * insert_pipe_lid_thickness, 0., twopi);
    G4LogicalVolume *insert_pipe_lid_logical = new G4LogicalVolume(
        insert_pipe_lid_solid, nist->FindOrBuildMaterial("G4_PLEXIGLASS"),
        "insert_pipe_lid_logical");
    new G4PVPlacement(0,
                      global_coordinates +
                          G4ThreeVector(0., 0.,
                                        beam_pipe_z + 0.5 * beam_pipe_length +
                                            0.5 * insert_pipe_lid_thickness),
                      insert_pipe_lid_logical, "insert_pipe_lid", world_logical,
                      false, 0, false);
  }

  if (use_target_ring) {
    G4Tubs *target_ring_solid = new G4Tubs(
        "target_ring_solid", insert_pipe_inner_radius,
        insert_pipe_front_inner_radius, 0.5 * target_ring_length, 0., twopi);
    G4LogicalVolume *target_ring_logical = new G4LogicalVolume(
        target_ring_solid, nist->FindOrBuildMaterial("G4_PLEXIGLASS"),
        "target_ring_logical");
    new G4PVPlacement(0,
                      global_coordinates +
                          G4ThreeVector(0., 0.,
                                        0.5 * beam_pipe_length -
                                            insert_pipe_main_length -
                                            0.5 * insert_pipe_front_length),
                      target_ring_logical, "target_ring",
                      beam_pipe_vacuum_logical, false, 0, false);
  }
}