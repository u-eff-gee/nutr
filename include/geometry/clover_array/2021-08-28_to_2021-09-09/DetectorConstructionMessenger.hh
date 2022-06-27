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

#pragma once

#include "G4UIcmdWithABool.hh"
#include "G4UImessenger.hh"

#include "DetectorConstruction.hh"

class DetectorConstruction;

class DetectorConstructionMessenger : public G4UImessenger {
public:
  DetectorConstructionMessenger(DetectorConstruction *detector_construction);
  void SetNewValue(G4UIcommand *command, G4String str) override;

protected:
  DetectorConstruction *detector_construction;

  G4UIcmdWithABool *activation_target_in_out_cmd;
};