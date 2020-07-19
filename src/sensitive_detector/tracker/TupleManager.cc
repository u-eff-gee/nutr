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
    analysisManager->CreateNtupleDColumn("posx");
    analysisManager->CreateNtupleDColumn("posy");
    analysisManager->CreateNtupleDColumn("posz");
}

void TupleManager::FillNtupleColumns(G4VAnalysisManager* analysisManager, G4int eventID, G4VHit* hit)
{
    analysisManager->FillNtupleIColumn(0, 0, eventID);
    analysisManager->FillNtupleIColumn(0, 1, ((DetectorHit*) hit)->GetTrackID());
    analysisManager->FillNtupleIColumn(0, 2, ((DetectorHit*) hit)->GetParticleID());
    analysisManager->FillNtupleIColumn(0, 3, ((DetectorHit*) hit)->GetDetectorID());
    analysisManager->FillNtupleDColumn(0, 4, ((DetectorHit*) hit)->GetEdep());
    analysisManager->FillNtupleDColumn(0, 5, ((DetectorHit*) hit)->GetPos().x());
    analysisManager->FillNtupleDColumn(0, 6, ((DetectorHit*) hit)->GetPos().y());
    analysisManager->FillNtupleDColumn(0, 7, ((DetectorHit*) hit)->GetPos().z());
}
