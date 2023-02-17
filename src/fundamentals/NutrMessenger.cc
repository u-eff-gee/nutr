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

#include <NutrMessenger.hh>

NutrMessenger::NutrMessenger()
    : dir("/analysis/"), cmd_filename("/analysis/filename", this) {
  dir.SetGuidance("Controls for general simulation settings.");

  cmd_filename.SetGuidance("Set filename of simulation output.");
  cmd_filename.SetParameterName("filename", false);
}

void NutrMessenger::SetNewValue(G4UIcommand *command, G4String str) {
  if (command == &cmd_filename) {
    filename = str;
  }
}
