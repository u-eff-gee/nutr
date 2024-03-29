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

#include <memory>

using std::dynamic_pointer_cast;

#include "G4RunManager.hh"

#include "DetectorHit.hh"
#include "NDetectorConstruction.hh"
#include "TupleManager.hh"

void TupleManager::CreateNtupleColumns(G4AnalysisManager *analysisManager) {

  analysisManager->CreateNtuple("edep", "Energy Deposition");

  AnalysisManager::CreateNtupleColumns(analysisManager);

  n_sensitive_detectors =
      ((NDetectorConstruction *)G4RunManager::GetRunManager()
           ->GetUserDetectorConstruction())
          ->GetNumberOfSensitiveDetectors();

  for (size_t i = 0; i < n_sensitive_detectors; ++i) {
    analysisManager->CreateNtupleDColumn("det" + to_string(i));
  }
}

size_t TupleManager::FillNtupleColumns(G4AnalysisManager *analysisManager,
                                       const G4Event *event,
                                       vector<G4VHit *> hits) {

  auto col = AnalysisManager::FillNtupleColumns(analysisManager, event, hits);

  for (size_t i = 0; i < hits.size(); ++i) {
    analysisManager->FillNtupleDColumn(
        0, col++, static_cast<DetectorHit *>(hits[i])->GetEdep());
  }
  // The number of entries in std::vector hits will only be as large as highest
  // ID of all detectors that were hit. There may be detectors with an even
  // higher ID which were not hit. Fill all higher IDs than hits.size()-1 with
  // zeros.
  for (size_t i = hits.size(); i < n_sensitive_detectors; ++i) {
    analysisManager->FillNtupleDColumn(0, col++, 0.);
  }
  return col;
}
