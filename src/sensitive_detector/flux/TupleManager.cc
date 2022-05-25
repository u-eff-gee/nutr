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
  analysisManager->CreateNtuple("part", "Particles");
  analysisManager->CreateNtupleIColumn("deid");
  analysisManager->CreateNtupleIColumn("pid");
  analysisManager->CreateNtupleIColumn("paid");
  analysisManager->CreateNtupleIColumn("trid");
  analysisManager->CreateNtupleDColumn("ekin");
  analysisManager->CreateNtupleDColumn("x");
  analysisManager->CreateNtupleDColumn("y");
  analysisManager->CreateNtupleDColumn("z");
  analysisManager->CreateNtupleDColumn("px");
  analysisManager->CreateNtupleDColumn("py");
  analysisManager->CreateNtupleDColumn("pz");
}

void TupleManager::FillNtupleColumns(G4AnalysisManager *analysisManager,
                                     [[maybe_unused]] int eventID,
                                     vector<shared_ptr<G4VHit>> hits) {
  analysisManager->FillNtupleIColumn(
      0, 0, dynamic_pointer_cast<DetectorHit>(hits[0])->GetDetectorID());
  analysisManager->FillNtupleIColumn(
      0, 1, dynamic_pointer_cast<DetectorHit>(hits[0])->GetParticleID());
  analysisManager->FillNtupleIColumn(
      0, 2, dynamic_pointer_cast<DetectorHit>(hits[0])->GetParentID());
  analysisManager->FillNtupleIColumn(
      0, 3, dynamic_pointer_cast<DetectorHit>(hits[0])->GetTrackID());
  analysisManager->FillNtupleDColumn(
      0, 4, dynamic_pointer_cast<DetectorHit>(hits[0])->GetEkin());
  analysisManager->FillNtupleDColumn(
      0, 5, dynamic_pointer_cast<DetectorHit>(hits[0])->GetPos().x());
  analysisManager->FillNtupleDColumn(
      0, 6, dynamic_pointer_cast<DetectorHit>(hits[0])->GetPos().y());
  analysisManager->FillNtupleDColumn(
      0, 7, dynamic_pointer_cast<DetectorHit>(hits[0])->GetPos().z());
  analysisManager->FillNtupleDColumn(
      0, 8, dynamic_pointer_cast<DetectorHit>(hits[0])->GetMom().x());
  analysisManager->FillNtupleDColumn(
      0, 9, dynamic_pointer_cast<DetectorHit>(hits[0])->GetMom().y());
  analysisManager->FillNtupleDColumn(
      0, 10, dynamic_pointer_cast<DetectorHit>(hits[0])->GetMom().z());
}
