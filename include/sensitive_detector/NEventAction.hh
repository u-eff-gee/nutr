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

#include "G4Event.hh"
#include "G4UserEventAction.hh"
#include "globals.hh"

#include "AnalysisManager.hh"
#include "SensitiveDetectorBuildOptions.hh"

class NEventAction : public G4UserEventAction {
public:
  NEventAction(AnalysisManager *ana_man)
      : G4UserEventAction(), analysis_manager(ana_man),
        update_frequency(sensitive_detector_build_options.update_frequency){};

  void BeginOfEventAction(const G4Event *event) override final;
  virtual void EndOfEventAction(const G4Event *) = 0;

protected:
  AnalysisManager *analysis_manager;
  const int update_frequency;
};
