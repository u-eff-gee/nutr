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
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

#include "ComptonMonitorTarget.hh"

void ComptonMonitorTarget::Construct(const G4ThreeVector global_coordinates) {

  G4NistManager *nist = G4NistManager::Instance();

  G4Box *scattering_target_solid = new G4Box(
      "scattering_target_solid", 0.5 * scattering_target_width,
      0.5 * scattering_target_height, 0.5 * scattering_target_thickness);
  G4LogicalVolume *scattering_target_logical = new G4LogicalVolume(
      scattering_target_solid, nist->FindOrBuildMaterial("G4_Cu"),
      "scattering_target_logical");
  scattering_target_logical->SetVisAttributes(G4Color(1., 165. / 255., 0));
  new G4PVPlacement(0, global_coordinates, scattering_target_logical,
                    "scattering_target", world_logical, false, 0, false);
}