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

#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

#include "ZeroDegreeMechanical.hh"

void ZeroDegreeMechanical::Construct(const G4ThreeVector global_coordinates) {
  // Crude implementation of the NaI annulus around the zero-degree detector for
  // the suppression of events where photons do not deposit their entire energy
  // in the crystal ("anti-Compton shield"). The only data available to the
  // author are the outer dimensions and the fact that it is a NaI crystal in an
  // aluminium housing.
  //
  // The zero-degree detector's front face is usually aligned with the front
  // face of the annulus.
  const double inch = 25.4 * mm;
  const double annulus_inner_radius = 2.125 * inch;
  const double annulus_outer_radius = annulus_inner_radius + 2.25 * inch;
  const double annulus_length = 7. * inch;
  const double annulus_rim_outer_radius = annulus_inner_radius + 3. * inch;
  const double annulus_rim_thickness = 1. * inch;
  const double annulus_inner_wall_thickness =
      2. *
      mm; // Estimated. The inner wall is probably thin to allow low-energy
          // photons from the zero-degree detector to reach the NaI crystal.
  const double annulus_outer_wall_thickness =
      0.75 *
      inch; // Estimated. The outer wall is probably much thicker than the inner
            // wall, because it needs to support the heavy crystal. Assumed the
            // thickness to be roughly on the order of the thickness of the rim.

  G4NistManager *nist = G4NistManager::Instance();

  G4Tubs *annulus_rim_solid = new G4Tubs(
      "annulus_rim_solid", annulus_inner_radius, annulus_rim_outer_radius,
      annulus_rim_thickness * 0.5, 0., twopi);
  G4LogicalVolume *annulus_upstream_rim_logical =
      new G4LogicalVolume(annulus_rim_solid, nist->FindOrBuildMaterial("G4_Al"),
                          "annulus_upstream_rim_logical");
  annulus_upstream_rim_logical->SetVisAttributes(G4Color::Gray());
  new G4PVPlacement(
      0,
      global_coordinates +
          G4ThreeVector(0., 0.,
                        zero_degree_to_target + 0.5 * annulus_rim_thickness),
      annulus_upstream_rim_logical, "annulus_upstream_rim", world_logical,
      false, 0, false);

  G4Tubs *annulus_body_solid =
      new G4Tubs("annulus_body_solid", annulus_inner_radius,
                 annulus_outer_radius, annulus_length * 0.5, 0., twopi);
  G4LogicalVolume *annulus_body_logical = new G4LogicalVolume(
      annulus_body_solid, nist->FindOrBuildMaterial("G4_Al"),
      "annulus_body_logical");
  annulus_body_logical->SetVisAttributes(G4Color::Gray());
  new G4PVPlacement(
      0,
      global_coordinates +
          G4ThreeVector(0., 0.,
                        zero_degree_to_target + annulus_rim_thickness +
                            0.5 * annulus_length),
      annulus_body_logical, "annulus_body", world_logical, false, 0, false);

  G4Tubs *nai_crystal_solid = new G4Tubs(
      "nai_crystal_solid", annulus_inner_radius + annulus_inner_wall_thickness,
      annulus_outer_radius - annulus_outer_wall_thickness, annulus_length * 0.5,
      0., twopi);
  G4LogicalVolume *nai_crystal_logical = new G4LogicalVolume(
      nai_crystal_solid, nist->FindOrBuildMaterial("G4_SODIUM_IODIDE"),
      "nai_crystal_logical");
  nai_crystal_logical->SetVisAttributes(G4Color::Cyan());
  new G4PVPlacement(0, G4ThreeVector(), nai_crystal_logical, "nai_crystal",
                    annulus_body_logical, false, 0, false);

  G4LogicalVolume *annulus_downstream_rim_logical =
      new G4LogicalVolume(annulus_rim_solid, nist->FindOrBuildMaterial("G4_Al"),
                          "annulus_downstream_rim_logical");
  annulus_downstream_rim_logical->SetVisAttributes(G4Color::Gray());
  new G4PVPlacement(0,
                    global_coordinates +
                        G4ThreeVector(0., 0.,
                                      zero_degree_to_target +
                                          1.5 * annulus_rim_thickness +
                                          annulus_length),
                    annulus_downstream_rim_logical, "annulus_downstream_rim",
                    world_logical, false, 0, false);
}