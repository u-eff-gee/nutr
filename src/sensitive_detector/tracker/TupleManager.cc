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

    Copyright (C) 2020, 2021 Udo Friman-Gayer
*/

#include "DetectorHit.hh"
#include "TupleManager.hh"

void TupleManager::CreateNtupleColumns(G4VAnalysisManager* analysisManager)
{
    analysisManager->CreateNtuple("hits", "Hits");
    analysisManager->CreateNtupleIColumn("evid");
    analysisManager->CreateNtupleIColumn("trid");
    analysisManager->CreateNtupleIColumn("paid");
    analysisManager->CreateNtupleIColumn("deid");
    analysisManager->CreateNtupleDColumn("edep");
    analysisManager->CreateNtupleDColumn("ekin");
    analysisManager->CreateNtupleDColumn("posx");
    analysisManager->CreateNtupleDColumn("posy");
    analysisManager->CreateNtupleDColumn("posz");
    analysisManager->CreateNtupleDColumn("momx");
    analysisManager->CreateNtupleDColumn("momy");
    analysisManager->CreateNtupleDColumn("momz");
}

void TupleManager::FillNtupleColumns(G4VAnalysisManager* analysisManager, G4int eventID, vector<G4VHit*> hits)
{
    analysisManager->FillNtupleIColumn(0, 0, eventID);
    analysisManager->FillNtupleIColumn(0, 1, ((DetectorHit*) hits[0])->GetTrackID());
    analysisManager->FillNtupleIColumn(0, 2, ((DetectorHit*) hits[0])->GetParticleID());
    analysisManager->FillNtupleIColumn(0, 3, ((DetectorHit*) hits[0])->GetDetectorID());
    analysisManager->FillNtupleDColumn(0, 4, ((DetectorHit*) hits[0])->GetEdep());
    analysisManager->FillNtupleDColumn(0, 5, ((DetectorHit*) hits[0])->GetEkin());
    analysisManager->FillNtupleDColumn(0, 6, ((DetectorHit*) hits[0])->GetPos().x());
    analysisManager->FillNtupleDColumn(0, 7, ((DetectorHit*) hits[0])->GetPos().y());
    analysisManager->FillNtupleDColumn(0, 8, ((DetectorHit*) hits[0])->GetPos().z());
    analysisManager->FillNtupleDColumn(0, 9, ((DetectorHit*) hits[0])->GetMom().x());
    analysisManager->FillNtupleDColumn(0,10, ((DetectorHit*) hits[0])->GetMom().y());
    analysisManager->FillNtupleDColumn(0,11, ((DetectorHit*) hits[0])->GetMom().z());
}
