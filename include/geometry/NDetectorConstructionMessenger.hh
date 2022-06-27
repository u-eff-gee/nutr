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
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIdirectory.hh"
#include "G4UImessenger.hh"

#include "NDetectorConstruction.hh"

class NDetectorConstruction;

class NDetectorConstructionMessenger : public G4UImessenger {
public:
  NDetectorConstructionMessenger(NDetectorConstruction *detector_construction);
  void SetNewValue(G4UIcommand *command, G4String str) override;

protected:
  NDetectorConstruction *detector_construction;

  G4UIdirectory *detector_construction_dir;
  G4UIcmdWithABool *molly_in_out_cmd;
  G4UIcmdWithABool *zero_degree_in_out_cmd;
  G4UIcmdWithADoubleAndUnit *zero_degree_y_cmd;
};