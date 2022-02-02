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

    Copyright (C) 2020-2022 Udo Friman-Gayer
*/

#include "G4SDManager.hh"

#include "SensitiveDetector.hh"

void SensitiveDetector::Initialize(G4HCofThisEvent* hce)
{

    fDetectorHitsCollection
      = new G4THitsCollection<DetectorHit>(SensitiveDetectorName, collectionName[0]);

    G4int hcID 
      = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hce->AddHitsCollection( hcID, fDetectorHitsCollection );
}

G4bool SensitiveDetector::ProcessHits(G4Step* aStep,
                                     G4TouchableHistory*)
{
  // Hits with no energy deposition are recorded as well.
  // This makes it possible to read out the point where a particle entered a detector volume, 
  // because movement ('transportation') counts as a 'hit' with an energy deposition of 0.

  DetectorHit* newDetectorHit = new DetectorHit();

  newDetectorHit->SetTrackID(aStep->GetTrack()->GetTrackID());
  newDetectorHit->SetParticleID(aStep->GetTrack()->GetDynamicParticle()->GetPDGcode());
  newDetectorHit->SetDetectorID(fDetectorID);
  newDetectorHit->SetGlobalTime(aStep->GetPreStepPoint()->GetGlobalTime());
  newDetectorHit->SetEdep(aStep->GetTotalEnergyDeposit());
  newDetectorHit->SetEnergy(aStep->GetPreStepPoint()->GetKineticEnergy());
  newDetectorHit->SetPos(aStep->GetPreStepPoint()->GetPosition());
  newDetectorHit->SetMom(aStep->GetPreStepPoint()->GetMomentum());

  fDetectorHitsCollection->insert( newDetectorHit );

  return true;
}
