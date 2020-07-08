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

#include "SensitiveDetector.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

SensitiveDetector::SensitiveDetector(const G4String& name,
                         const G4String& DetectorHitsCollectionName)
 : G4VSensitiveDetector(name),
   fDetectorHitsCollection(NULL)
{
  collectionName.insert(DetectorHitsCollectionName);
}

SensitiveDetector::~SensitiveDetector()
{}

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
  G4double edep = aStep->GetTotalEnergyDeposit();
  if (edep==0.) return false;

  DetectorHit* newDetectorHit = new DetectorHit();

  newDetectorHit->SetDetectorID(fDetectorID);
  newDetectorHit->SetEdep(edep);

  fDetectorHitsCollection->insert( newDetectorHit );

  return true;
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{}
