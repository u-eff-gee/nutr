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

    Copyright (C) 2020 Udo Friman-Gayer
*/

#include "TupleManager.hh"

TupleManager::TupleManager()
: fFactoryOn(false)
{}

TupleManager::~TupleManager()
{}

void TupleManager::Book()
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetNtupleMerging(true);

  G4bool fileOpen = analysisManager->OpenFile("out");
  if (! fileOpen) {
    G4cerr << "\n---> HistoManager::Book(): cannot open "
           << analysisManager->GetFileName() << G4endl;
    return;
  }

  analysisManager->CreateNtuple("hits", "Hits");
  analysisManager->CreateNtupleIColumn("evid");
  analysisManager->CreateNtupleIColumn("trid");
  analysisManager->CreateNtupleIColumn("paid");
  analysisManager->CreateNtupleIColumn("deid");
  analysisManager->CreateNtupleDColumn("edep");
  analysisManager->CreateNtupleDColumn("posx");
  analysisManager->CreateNtupleDColumn("posy");
  analysisManager->CreateNtupleDColumn("posz");
  analysisManager->FinishNtuple();

  fFactoryOn = true;

}

void TupleManager::Save()
{
  if (! fFactoryOn) return;

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();

  delete G4AnalysisManager::Instance();
  fFactoryOn = false;
}

void TupleManager::FillNtuple(G4int eventID, TrackerHit* hit)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleIColumn(0, 0, eventID);
  analysisManager->FillNtupleIColumn(0, 1, hit->GetTrackID());
  analysisManager->FillNtupleIColumn(0, 2, hit->GetParticleID());
  analysisManager->FillNtupleIColumn(0, 3, hit->GetDetectorID());
  analysisManager->FillNtupleDColumn(0, 4, hit->GetEdep());
  analysisManager->FillNtupleDColumn(0, 5, hit->GetPos().x());
  analysisManager->FillNtupleDColumn(0, 6, hit->GetPos().y());
  analysisManager->FillNtupleDColumn(0, 7, hit->GetPos().z());

  analysisManager->AddNtupleRow(0);
}
