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

    analysisManager->CreateNtuple("edep", "Energy Deposition");
    analysisManager->CreateNtupleIColumn("deid");
    analysisManager->CreateNtupleDColumn("edep");

}

void TupleManager::FillNtupleColumns(G4VAnalysisManager* analysisManager, [[maybe_unused]] G4int eventID, vector<G4VHit*> hits)
{

    analysisManager->FillNtupleIColumn(0, 0, ((DetectorHit*) hits[0])->GetDetectorID());
    analysisManager->FillNtupleDColumn(0, 1, ((DetectorHit*) hits[0])->GetEdep());

}
