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

#include "GammaVault.hh"

void GammaVault::Construct(const G4ThreeVector global_coordinates) {

  G4NistManager *nist = G4NistManager::Instance();

  const double inch = 25.4 * mm;
  const double utr_gv_wall_to_target =
      4.45 * m; // The length of the UTR (wall to wall) is 6.28 m. The 4.45
                // meters here from the target position to the downstream are
                // consistent with the 72 inches from the target position to the
                // upstream wall in the collimator-room class.
  const double utr_gv_wall_thickness =
      600. * mm; // Thickness of the permanent wall.
  const double utr_gv_wall_width =
      2000. * mm; // Estimated, width and height should be large enough to take
                  // into account the effect of the wall on the HIγS beam.
  const double utr_gv_wall_height = 2000. * mm;     // Estimated
  const double utr_gv_wall_hole_radius = 2. * inch; // Estimated

  G4Box *utr_gv_wall_without_hole_solid =
      new G4Box("utr_gv_wall_without_hole_solid", 0.5 * utr_gv_wall_width,
                0.5 * utr_gv_wall_height, 0.5 * utr_gv_wall_thickness);
  G4SubtractionSolid *utr_gv_wall_solid = new G4SubtractionSolid(
      "utr_gv_wall_solid", utr_gv_wall_without_hole_solid,
      new G4Tubs("utr_gv_wall_hole", 0., utr_gv_wall_hole_radius,
                 utr_gv_wall_thickness, 0., twopi));
  G4LogicalVolume *utr_gv_wall_logical = new G4LogicalVolume(
      utr_gv_wall_solid, nist->FindOrBuildMaterial("G4_CONCRETE"),
      "utr_gv_wall_logical");
  utr_gv_wall_logical->SetVisAttributes(G4Color::White());
  new G4PVPlacement(
      0,
      global_coordinates +
          G4ThreeVector(0., 0.,
                        utr_gv_wall_to_target + 0.5 * utr_gv_wall_thickness),
      utr_gv_wall_logical, "utr_gv_wall", world_logical, false, 0, false);
}