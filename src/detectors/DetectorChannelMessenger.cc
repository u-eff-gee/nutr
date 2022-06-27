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

#include <string>

using std::to_string;

#include "DetectorChannelMessenger.hh"

DetectorChannelMessenger::DetectorChannelMessenger(Detector *_detector)
    : detector(_detector), n_channel(0) {

  string parameter_name = detector->detector_name + "_dead_layer";
  dead_layer_cmd =
      new G4UIcmdWithADouble(("/nutr/" + parameter_name).c_str(), this);
  dead_layer_cmd->SetGuidance(
      ("Set the scaling factor for the dead layer of detector '" +
       detector->detector_name +
       "'. The value of this parameter should be between 0.0 (default, no dead "
       "layer, entire detector is sensitive to radiation) and 1.0 (entire "
       "detector is dead).")
          .c_str());
  dead_layer_cmd->SetParameterName(parameter_name.c_str(), false);
  dead_layer_cmd->SetDefaultValue(0.);
}

DetectorChannelMessenger::DetectorChannelMessenger(Detector *_detector,
                                                   const size_t _n_channel)
    : detector(_detector), n_channel(_n_channel) {

  string parameter_name =
      detector->detector_name + "_" + to_string(n_channel + 1) + "_dead_layer";
  dead_layer_cmd =
      new G4UIcmdWithADouble(("/nutr/" + parameter_name).c_str(), this);
  dead_layer_cmd->SetGuidance(
      ("Set the scaling factor for the dead layer of channel #" +
       to_string(n_channel + 1) + " of detector '" + detector->detector_name +
       "'. The value of this parameter should be between 0.0 (default, no dead "
       "layer, entire detector is sensitive to radiation) and 1.0 (entire "
       "detector is dead).")
          .c_str());
  dead_layer_cmd->SetParameterName(parameter_name.c_str(), false);
  dead_layer_cmd->SetDefaultValue(0.);
}

void DetectorChannelMessenger::SetNewValue(G4UIcommand *command, G4String str) {
  if (command == dead_layer_cmd) {
    detector->set_dead_layer(n_channel, dead_layer_cmd->GetNewDoubleValue(str));
  }
}