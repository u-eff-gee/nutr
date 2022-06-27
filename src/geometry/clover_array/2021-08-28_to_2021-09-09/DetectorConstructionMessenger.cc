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

#include "DetectorConstructionMessenger.hh"

DetectorConstructionMessenger::DetectorConstructionMessenger(
    DetectorConstruction *_detector_construction)
    : detector_construction(_detector_construction) {

  activation_target_in_out_cmd =
      new G4UIcmdWithABool("/nutr/activation_target", this);
  activation_target_in_out_cmd->SetGuidance(
      "Determines whether the activation target is used (true) or not (false) "
      " (default: true).");
  activation_target_in_out_cmd->SetParameterName("in_out", false);
  activation_target_in_out_cmd->SetDefaultValue(true);
  activation_target_in_out_cmd->AvailableForStates(G4State_PreInit);
}

void DetectorConstructionMessenger::SetNewValue(G4UIcommand *command,
                                                G4String str) {
  if (command == activation_target_in_out_cmd) {
    detector_construction->activation_target_in_out(
        activation_target_in_out_cmd->GetNewBoolValue(str));
  }
}