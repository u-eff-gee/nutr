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

    Copyright (C) 2021 Oliver Papst
*/

#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4Isotope.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

#include "Target96Mo.hh"

void Target96Mo::Construct(const G4ThreeVector global_coordinates) {

  auto darkgray = G4Colour(0.2, 0.2, 0.2);
  auto lightgray = G4Colour(0.6, 0.6, 0.6, 0.5);
  auto doublelightgray = G4Colour(0.6, 0.6, 0.6, 0.75);

  G4NistManager *nist = G4NistManager::Instance();
  auto *material_pe = nist->FindOrBuildMaterial("G4_POLYETHYLENE");

  auto radius_96mo = 0.5 * diameter_96mo;
  auto thickness_pe = (0.5 * mass_pe / density_pe) / (side_pe * side_pe);
  auto thickness_96mo = thickness_tot - 2 * thickness_pe;
  auto mass_pe_disc =
      pi * radius_96mo * radius_96mo * thickness_pe * density_pe;
  auto volume_pe_shell = 2 * pi * radius_96mo * thickness_pe * thickness_96mo;
  auto mass_pe_shell = volume_pe_shell * density_pe;
  auto mass_pe_squarehole = mass_pe - 2 * mass_pe_disc - mass_pe_shell;
  auto radius_pe_hole = radius_96mo + thickness_pe;
  auto area_pe_squarehole =
      side_pe * side_pe - pi * radius_pe_hole * radius_pe_hole;
  auto thickness_pe_squarehole =
      mass_pe_squarehole / density_pe / area_pe_squarehole;
  auto radius_pe_disc = radius_96mo + thickness_pe;
  auto area_pe_disc = pi * radius_pe_disc * radius_pe_disc;
  auto thickness_pe_disc = mass_pe_disc / density_pe / area_pe_disc;

  G4Isotope *Mo92 = new G4Isotope("92Mo", 42, 92);
  G4Isotope *Mo94 = new G4Isotope("94Mo", 42, 94);
  G4Isotope *Mo95 = new G4Isotope("95Mo", 42, 95);
  G4Isotope *Mo96 = new G4Isotope("96Mo", 42, 96);
  G4Isotope *Mo97 = new G4Isotope("97Mo", 42, 97);
  G4Isotope *Mo98 = new G4Isotope("98Mo", 42, 98);
  G4Isotope *Mo100 = new G4Isotope("100Mo", 42, 100);

  G4Element *enriched_96Mo = new G4Element("enriched 96Mo", "96Mo-hzdr", 7);
  enriched_96Mo->AddIsotope(Mo92, 0.0028);
  enriched_96Mo->AddIsotope(Mo94, 0.0024);
  enriched_96Mo->AddIsotope(Mo95, 0.0101);
  enriched_96Mo->AddIsotope(Mo96, 0.9590);
  enriched_96Mo->AddIsotope(Mo97, 0.0100);
  enriched_96Mo->AddIsotope(Mo98, 0.0132);
  enriched_96Mo->AddIsotope(Mo100, 0.0025);

  auto density_96mo = mass_96mo / (radius_96mo * radius_96mo * pi);
  auto *material_96mo = new G4Material("mat_96Mo", density_96mo, 1);
  material_96mo->AddElement(enriched_96Mo, 1);

  auto *target_96Mo_solid = new G4Tubs("target_96Mo_solid", 0., radius_96mo,
                                       0.5 * thickness_96mo, 0., 2. * pi);

  auto *target_pe_top_solid =
      new G4Tubs("target_pe_top_solid", 0., radius_pe_disc,
                 0.5 * thickness_pe_disc, 0., 2. * pi);
  auto *target_pe_bottom_solid =
      new G4Tubs("target_pe_bottom_solid", 0., radius_pe_disc,
                 0.5 * thickness_pe_disc, 0., 2. * pi);
  auto *target_pe_square_solid =
      new G4Box("target_pe_square", 0.5 * side_pe, 0.5 * side_pe,
                0.5 * thickness_pe_squarehole);
  auto *target_pe_hole_solid = new G4Tubs("target_pe_hole", 0., radius_pe_hole,
                                          thickness_pe_squarehole, 0., 2. * pi);
  auto *target_pe_squarehole_solid = new G4SubtractionSolid(
      "target_pe_squarehole", target_pe_square_solid, target_pe_hole_solid);
  auto *target_pe_shell_solid =
      new G4Tubs("target_pe_shell", radius_96mo, radius_pe_hole,
                 0.5 * thickness_96mo, 0., 2. * pi);

  auto *target_96Mo_logical = new G4LogicalVolume(
      target_96Mo_solid, material_96mo, "target_96Mo_logical");
  target_96Mo_logical->SetVisAttributes(darkgray);

  auto *target_pe_top_logical = new G4LogicalVolume(
      target_pe_top_solid, material_pe, "target_pe_top_logical");
  target_pe_top_logical->SetVisAttributes(lightgray);
  auto *target_pe_bottom_logical = new G4LogicalVolume(
      target_pe_bottom_solid, material_pe, "target_pe_bottom_logical");
  target_pe_bottom_logical->SetVisAttributes(lightgray);
  auto *target_pe_squarehole_logical = new G4LogicalVolume(
      target_pe_squarehole_solid, material_pe, "target_pe_squarehole_logical");
  target_pe_squarehole_logical->SetVisAttributes(doublelightgray);
  auto *target_pe_shell_logical = new G4LogicalVolume(
      target_pe_shell_solid, material_pe, "target_pe_shell_logical");
  target_pe_shell_logical->SetVisAttributes(lightgray);

  new G4PVPlacement(0, global_coordinates, target_96Mo_logical, "target_96Mo",
                    world_logical, false, 0, false);
  new G4PVPlacement(0, global_coordinates, target_pe_shell_logical,
                    "target_pe_shell", world_logical, false, 0, false);
  new G4PVPlacement(0, global_coordinates, target_pe_squarehole_logical,
                    "target_pe_squarehole", world_logical, false, 0, false);
  new G4PVPlacement(
      0,
      global_coordinates +
          G4ThreeVector(0., 0., 0.5 * (thickness_96mo + thickness_pe_disc)),
      target_pe_top_logical, "target_pe_top", world_logical, false, 0, false);
  new G4PVPlacement(
      0,
      global_coordinates +
          G4ThreeVector(0., 0., -0.5 * (thickness_96mo + thickness_pe_disc)),
      target_pe_bottom_logical, "target_pe_bottom", world_logical, false, 0,
      false);
}
