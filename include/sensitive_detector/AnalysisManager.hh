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

// Based on Geant4 10.6.1 example
//
// ${CMAKE_INSTALL_PREFIX}/examples/extended/analysis/AnaEx02

#pragma once

#include <memory>

using std::shared_ptr;

#include <string>
#include <vector>

using std::string;
using std::vector;

#include "G4AnalysisManager.hh"
#include "G4Event.hh"
#include "G4VHit.hh"
#include "globals.hh"

class AnalysisManager {
public:
  AnalysisManager();
  ~AnalysisManager();

  void Book(string output_file_name);
  virtual void CreateNtupleColumns(G4AnalysisManager *analysisManager) = 0;
  virtual void FillNtuple(const G4Event *event,
                          vector<shared_ptr<G4VHit>> hits);
  virtual void FillNtupleColumns(G4AnalysisManager *analysisManager,
                                 const G4Event *event,
                                 vector<shared_ptr<G4VHit>> hits) = 0;
  void Save();

protected:
  string create_default_file_name() const;
  G4bool fFactoryOn;
};
