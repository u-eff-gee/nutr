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

#include "TupleManager.hh"
#include "DetectorHit.hh"

void TupleManager::CreateNtupleColumns(G4AnalysisManager *analysisManager) {

  analysisManager->CreateNtuple("edep", "Energy Deposition");
  analysisManager->CreateNtupleIColumn("deid");
  analysisManager->CreateNtupleDColumn("edep");
  analysisManager->CreateNtupleDColumn("x0");
  analysisManager->CreateNtupleDColumn("y0");
  analysisManager->CreateNtupleDColumn("z0");
  analysisManager->CreateNtupleDColumn("px0");
  analysisManager->CreateNtupleDColumn("py0");
  analysisManager->CreateNtupleDColumn("pz0");
  analysisManager->CreateNtupleDColumn("E0");
}

void TupleManager::FillNtupleColumns(G4AnalysisManager *analysisManager,
                                     [[maybe_unused]] int eventID,
                                     vector<shared_ptr<G4VHit>> hits,
                                     G4PrimaryVertex *primary_vertex) {

  analysisManager->FillNtupleIColumn(
      0, 0, dynamic_pointer_cast<DetectorHit>(hits[0])->GetDetectorID());
  analysisManager->FillNtupleDColumn(
      0, 1, dynamic_pointer_cast<DetectorHit>(hits[0])->GetEdep());
  analysisManager->FillNtupleDColumn(0, 2, primary_vertex->GetPosition().x());
  analysisManager->FillNtupleDColumn(0, 3, primary_vertex->GetPosition().y());
  analysisManager->FillNtupleDColumn(0, 4, primary_vertex->GetPosition().z());
  analysisManager->FillNtupleDColumn(
      0, 5, primary_vertex->GetPrimary(0)->GetMomentumDirection().x());
  analysisManager->FillNtupleDColumn(
      0, 6, primary_vertex->GetPrimary(0)->GetMomentumDirection().y());
  analysisManager->FillNtupleDColumn(
      0, 7, primary_vertex->GetPrimary(0)->GetMomentumDirection().z());
  analysisManager->FillNtupleDColumn(
      0, 8, primary_vertex->GetPrimary(0)->GetKineticEnergy());
}
