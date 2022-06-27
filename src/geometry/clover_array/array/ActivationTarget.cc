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

#include "ActivationTarget.hh"

void ActivationTarget::Construct(const G4ThreeVector global_coordinates) {
  const double inch = 25.4 * mm;
  G4NistManager *nist = G4NistManager::Instance();

  // Dimensions of the capsule taken from a technical drawing supplied by S.
  // Finch.
  const double capsule_inner_radius = 0.5 * 1.040 * inch;
  const double capsule_outer_radius = 0.5 * 1.500 * inch;
  const double capsule_lid_thickness = 0.063 * inch;
  const double capsule_bottom_thickness = 0.055 * inch;
  const double capsule_chamber_length = 0.483 * inch;
  const double capsule_total_length =
      capsule_bottom_thickness + capsule_chamber_length + capsule_lid_thickness;

  // Dimensions of the holder and the rod were measured with a caliper.
  const double activation_target_holder_to_target = 54.0 * inch;
  const double holder_length = 0.7 * inch;
  const double holder_groove_length = capsule_total_length;
  const double holder_width = 1.38 * inch;
  const double holder_height = 0.8 * inch;
  const double holder_groove_depth = 0.22 * inch;
  const double holder_groove_radius = capsule_outer_radius;

  const double base_rod_diameter = 0.5 * inch;
  const double base_rod_height = 100. * mm; // Estimated
  const double holder_y = holder_groove_radius - holder_groove_depth;

  // Holder
  G4Box *holder_full_solid = new G4Box("holder_full_solid", holder_width / 2.,
                                       holder_height / 2., holder_length / 2.);
  G4Tubs *holder_groove_solid =
      new G4Tubs("holder_groove_solid", 0., holder_groove_radius,
                 holder_groove_length / 2., 0., twopi);
  G4SubtractionSolid *holder_solid = new G4SubtractionSolid(
      "holder_solid", holder_full_solid, holder_groove_solid, nullptr,
      G4ThreeVector(
          0., holder_height / 2. + holder_groove_radius - holder_groove_depth,
          0.));
  G4LogicalVolume *holder_logical = new G4LogicalVolume(
      holder_solid, nist->FindOrBuildMaterial("G4_POLYETHYLENE"),
      "holder_logical");
  holder_logical->SetVisAttributes(G4Color::White());
  new G4PVPlacement(
      0,
      global_coordinates + G4ThreeVector(0, -(holder_y + holder_height / 2.),
                                         activation_target_holder_to_target),
      holder_logical, "activation_target_holder", world_logical, false, 0);

  // Rod
  G4Tubs *base_rod_solid =
      new G4Tubs("base_rod_solid", 0., base_rod_diameter / 2.,
                 base_rod_height / 2., 0., twopi);
  G4LogicalVolume *base_rod_logical = new G4LogicalVolume(
      base_rod_solid, nist->FindOrBuildMaterial("G4_Al"), "base_rod_logical");
  base_rod_logical->SetVisAttributes(G4Color::Gray());
  new G4PVPlacement(
      new G4RotationMatrix(G4ThreeVector(1., 0., 0.), 90. * degree),
      global_coordinates +
          G4ThreeVector(0, -(holder_y + holder_height + base_rod_height / 2.),
                        activation_target_holder_to_target),
      base_rod_logical, "activation_target_base_rod", world_logical, false, 0);

  // Capsule
  G4Tubs *capsule_bottom_solid =
      new G4Tubs("capsule_solid", 0., capsule_outer_radius,
                 0.5 * capsule_bottom_thickness, 0., twopi);
  G4LogicalVolume *capsule_bottom_logical = new G4LogicalVolume(
      capsule_bottom_solid, nist->FindOrBuildMaterial("G4_PLEXIGLASS"),
      "capsule_bottom_logical");
  capsule_bottom_logical->SetVisAttributes(G4Color::White());
  new G4PVPlacement(
      0,
      global_coordinates + G4ThreeVector(0., 0.,
                                         activation_target_holder_to_target -
                                             0.5 * capsule_total_length +
                                             0.5 * capsule_bottom_thickness),
      capsule_bottom_logical, "capsule_lid", world_logical, false, 0);

  G4Tubs *capsule_chamber_solid =
      new G4Tubs("capsule_solid", capsule_inner_radius, capsule_outer_radius,
                 0.5 * capsule_chamber_length, 0., twopi);
  G4LogicalVolume *capsule_chamber_logical = new G4LogicalVolume(
      capsule_chamber_solid, nist->FindOrBuildMaterial("G4_PLEXIGLASS"),
      "capsule_chamber_logical");
  capsule_chamber_logical->SetVisAttributes(G4Color::White());
  new G4PVPlacement(
      0,
      global_coordinates + G4ThreeVector(0., 0.,
                                         activation_target_holder_to_target -
                                             0.5 * capsule_total_length +
                                             capsule_bottom_thickness +
                                             0.5 * capsule_chamber_length),
      capsule_chamber_logical, "capsule_chamber", world_logical, false, 0);

  G4Tubs *capsule_lid_solid =
      new G4Tubs("capsule_solid", 0., capsule_outer_radius,
                 0.5 * capsule_lid_thickness, 0., twopi);
  G4LogicalVolume *capsule_lid_logical = new G4LogicalVolume(
      capsule_lid_solid, nist->FindOrBuildMaterial("G4_PLEXIGLASS"),
      "capsule_lid_logical");
  capsule_lid_logical->SetVisAttributes(G4Color::White());
  new G4PVPlacement(
      0,
      global_coordinates + G4ThreeVector(0., 0.,
                                         activation_target_holder_to_target +
                                             0.5 * capsule_total_length -
                                             0.5 * capsule_lid_thickness),
      capsule_lid_logical, "capsule_lid", world_logical, false, 0);
}