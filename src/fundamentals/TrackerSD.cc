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

#include "TrackerSD.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

TrackerSD::TrackerSD(const G4String& name,
                         const G4String& TrackerHitsCollectionName)
 : G4VSensitiveDetector(name),
   fTrackerHitsCollection(NULL)
{
  collectionName.insert(TrackerHitsCollectionName);
}

TrackerSD::~TrackerSD()
{}

void TrackerSD::Initialize(G4HCofThisEvent* hce)
{

  fTrackerHitsCollection
    = new G4THitsCollection<TrackerHit>(SensitiveDetectorName, collectionName[0]);

  G4int hcID 
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fTrackerHitsCollection );
}

G4bool TrackerSD::ProcessHits(G4Step* aStep,
                                     G4TouchableHistory*)
{
  G4double edep = aStep->GetTotalEnergyDeposit();

  if (edep==0.) return false;

  TrackerHit* newTrackerHit = new TrackerHit();

  newTrackerHit->SetTrackID(aStep->GetTrack()->GetTrackID());
  newTrackerHit->SetParticleID(aStep->GetTrack()->GetDynamicParticle()->GetPDGcode());
  newTrackerHit->SetDetectorID(fDetectorID);
  newTrackerHit->SetEdep(edep);
  newTrackerHit->SetPos(aStep->GetPostStepPoint()->GetPosition());

  fTrackerHitsCollection->insert( newTrackerHit );

  return true;
}

void TrackerSD::EndOfEvent(G4HCofThisEvent*)
{}
