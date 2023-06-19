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
  AnalysisManager::CreateNtupleColumns(analysisManager);
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

size_t TupleManager::FillNtupleColumns(G4AnalysisManager *analysisManager,
                                       [[maybe_unused]] const G4Event *event,
                                       vector<G4VHit *> hits) {

  auto col = AnalysisManager::FillNtupleColumns(analysisManager, event, hits);

  analysisManager->FillNtupleIColumn(
      0, col++, static_cast<DetectorHit *>(hits[0])->GetDetectorID());
  analysisManager->FillNtupleIColumn(
      0, col++, static_cast<DetectorHit *>(hits[0])->GetParticleID());
  analysisManager->FillNtupleIColumn(
      0, col++, static_cast<DetectorHit *>(hits[0])->GetParentID());
  analysisManager->FillNtupleIColumn(
      0, col++, static_cast<DetectorHit *>(hits[0])->GetTrackID());
  analysisManager->FillNtupleDColumn(
      0, col++, static_cast<DetectorHit *>(hits[0])->GetEkin());
  analysisManager->FillNtupleDColumn(
      0, col++, static_cast<DetectorHit *>(hits[0])->GetPos().x());
  analysisManager->FillNtupleDColumn(
      0, col++, static_cast<DetectorHit *>(hits[0])->GetPos().y());
  analysisManager->FillNtupleDColumn(
      0, col++, static_cast<DetectorHit *>(hits[0])->GetPos().z());
  analysisManager->FillNtupleDColumn(
      0, col++, static_cast<DetectorHit *>(hits[0])->GetMom().x());
  analysisManager->FillNtupleDColumn(
      0, col++, static_cast<DetectorHit *>(hits[0])->GetMom().y());
  analysisManager->FillNtupleDColumn(
      0, col++, static_cast<DetectorHit *>(hits[0])->GetMom().z());

  return col;
}
