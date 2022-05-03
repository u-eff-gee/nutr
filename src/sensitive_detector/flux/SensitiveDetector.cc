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

void SensitiveDetector::Initialize(G4HCofThisEvent *hce) {

  fDetectorHitsCollection = new G4THitsCollection<DetectorHit>(
      SensitiveDetectorName, collectionName[0]);

  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection(hcID, fDetectorHitsCollection);
}

G4bool SensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *) {
  DetectorHit *newDetectorHit = new DetectorHit();

  newDetectorHit->SetDetectorID(fDetectorID);
  newDetectorHit->SetParticleID(
      aStep->GetTrack()->GetDynamicParticle()->GetPDGcode());
  newDetectorHit->SetParentID(aStep->GetTrack()->GetParentID());
  newDetectorHit->SetTrackID(aStep->GetTrack()->GetTrackID());
  newDetectorHit->SetEkin(aStep->GetTrack()->GetKineticEnergy());
  newDetectorHit->SetPos(aStep->GetPostStepPoint()->GetPosition());
  newDetectorHit->SetMom(aStep->GetTrack()->GetMomentum());

  fDetectorHitsCollection->insert(newDetectorHit);

  return true;
}