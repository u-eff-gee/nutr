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

#include "G4SystemOfUnits.hh"

#include "NDetectorConstructionMessenger.hh"

NDetectorConstructionMessenger::NDetectorConstructionMessenger(
    NDetectorConstruction *_detector_construction)
    : detector_construction(_detector_construction) {
  detector_construction_dir = new G4UIdirectory("/nutr/");
  detector_construction_dir->SetGuidance(
      "Settings specific to the geometries implemented in the nutr "
      "application.");

  molly_in_out_cmd = new G4UIcmdWithABool("/nutr/molly", this);
  molly_in_out_cmd->SetGuidance(
      "Determines whether MOLLY detector is in its IN (true) or OUT (false) "
      "position (default: true).");
  molly_in_out_cmd->SetParameterName("in_out", false);
  molly_in_out_cmd->SetDefaultValue(true);
  molly_in_out_cmd->AvailableForStates(G4State_PreInit);

  zero_degree_in_out_cmd = new G4UIcmdWithABool("/nutr/zero_degree", this);
  zero_degree_in_out_cmd->SetGuidance(
      "Determines whether zero-degree detector is in its IN (true) or OUT "
      "(false) position (default: true).");
  zero_degree_in_out_cmd->SetParameterName("in_out", false);
  zero_degree_in_out_cmd->SetDefaultValue(true);
  zero_degree_in_out_cmd->AvailableForStates(G4State_PreInit);

  zero_degree_y_cmd =
      new G4UIcmdWithADoubleAndUnit("/nutr/zero_degree_y", this);
  zero_degree_y_cmd->SetGuidance(
      "Set offset of zero-degree detector on y axis (default: 30 mm).");
  zero_degree_y_cmd->SetParameterName("y", false);
  zero_degree_y_cmd->SetUnitCategory("Length");
  zero_degree_y_cmd->AvailableForStates(G4State_PreInit);
}

void NDetectorConstructionMessenger::SetNewValue(G4UIcommand *command,
                                                 G4String str) {
  if (command == molly_in_out_cmd) {
    detector_construction->set_molly_x(
        molly_in_out_cmd->GetNewBoolValue(str) ? 0. : -500. * mm);
  }
  if (command == zero_degree_in_out_cmd) {
    detector_construction->set_zero_degree_x(
        zero_degree_in_out_cmd->GetNewBoolValue(str) ? 0. : -500. * mm);
  }
  if (command == zero_degree_y_cmd) {
    detector_construction->set_zero_degree_y(
        zero_degree_y_cmd->GetNewDoubleValue(str));
  }
}