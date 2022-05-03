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

    Copyright (C) 2020, 2021 Udo Friman-Gayer
*/

#include "G4Box.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

#include "ComptonMonitor_2021-09-09_to_2021-10-10.hh"

void ComptonMonitor::Construct(const G4ThreeVector global_coordinates) {
  (void)global_coordinates;

  const double inch = 25.4 * mm;

  const double lead_shielding_side_length = 8. * inch;
  const double lead_shielding_side_thickness = 4. * inch;
  const double lead_shielding_side_height = 6. * inch;

  const double lead_shielding_top_width = 16. * inch;
  const double lead_shielding_top_height = 4. * inch;
  const double lead_shielding_top_thickness = 4. * inch;

  G4NistManager *nist = G4NistManager::Instance();

  // Detector shielding

  const double shielding_rotation_angle = 9. * deg;
  const G4ThreeVector shielding_rotation_center(
      50. * cm * sin(detector_angle), 0., 50. * cm * cos(detector_angle));
  const double shielding_to_rotation_center = 20. * inch;
  const double shielding_side_y =
      -0.5 * lead_shielding_side_height + 1.8 * inch;
  G4Box *lead_shielding_side_solid = new G4Box(
      "lead_shielding_side_solid", 0.5 * lead_shielding_side_thickness,
      0.5 * lead_shielding_side_height, 0.5 * lead_shielding_side_length);
  G4LogicalVolume *lead_shielding_side_logical = new G4LogicalVolume(
      lead_shielding_side_solid, nist->FindOrBuildMaterial("G4_Pb"),
      "lead_shielding_side_logical");
  lead_shielding_side_logical->SetVisAttributes(G4Color::Green());
  G4RotationMatrix *rotate_left_shielding = new G4RotationMatrix();
  rotate_left_shielding->rotateY(-detector_angle - shielding_rotation_angle);
  new G4PVPlacement(
      rotate_left_shielding,
      shielding_rotation_center +
          G4ThreeVector(shielding_to_rotation_center *
                            sin(shielding_rotation_angle + detector_angle),
                        shielding_side_y,
                        shielding_to_rotation_center *
                            cos(shielding_rotation_angle + detector_angle)),
      lead_shielding_side_logical, "lead_shielding_left", world_logical, false,
      0, false);

  G4RotationMatrix *rotate_right_shielding = new G4RotationMatrix();
  rotate_right_shielding->rotateY(-detector_angle + shielding_rotation_angle);
  new G4PVPlacement(
      rotate_right_shielding,
      shielding_rotation_center +
          G4ThreeVector(shielding_to_rotation_center *
                            sin(-shielding_rotation_angle + detector_angle),
                        shielding_side_y,
                        shielding_to_rotation_center *
                            cos(-shielding_rotation_angle + detector_angle)),
      lead_shielding_side_logical, "lead_shielding_right", world_logical, false,
      0, false);

  G4Box *lead_shielding_top_solid = new G4Box(
      "lead_shielding_top_solid", 0.5 * lead_shielding_top_width,
      0.5 * lead_shielding_top_height, 0.5 * lead_shielding_top_thickness);
  G4LogicalVolume *lead_shielding_top_logical = new G4LogicalVolume(
      lead_shielding_top_solid, nist->FindOrBuildMaterial("G4_Pb"),
      "lead_shielding_top_logical");
  lead_shielding_top_logical->SetVisAttributes(G4Color::Green());
  G4RotationMatrix *rotate_top_shielding = new G4RotationMatrix();
  rotate_top_shielding->rotateY(-detector_angle);
  new G4PVPlacement(
      rotate_top_shielding,
      shielding_rotation_center +
          G4ThreeVector(
              0.9 * shielding_to_rotation_center * sin(detector_angle),
              shielding_side_y + 0.5 * lead_shielding_side_height +
                  0.5 * lead_shielding_top_height,
              0.9 * shielding_to_rotation_center * cos(detector_angle)),
      lead_shielding_top_logical, "lead_shielding_top", world_logical, false, 0,
      false);
}
