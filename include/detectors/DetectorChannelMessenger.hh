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

#include "G4UIcmdWithADouble.hh"
#include "G4UImessenger.hh"

#include "Detector.hh"

class Detector;

class DetectorChannelMessenger : public G4UImessenger {
public:
  DetectorChannelMessenger(Detector *_detector);
  DetectorChannelMessenger(Detector *_detector, const size_t _n_channel);
  void SetNewValue(G4UIcommand *command, G4String str) override;

protected:
  Detector *detector;

  const size_t n_channel;
  G4UIcmdWithADouble *dead_layer_cmd;
};