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
#pragma once

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4VSensitiveDetector.hh"

#include "TrackerHit.hh"

class TrackerSD : public G4VSensitiveDetector
{
  public:
    TrackerSD(const G4String& name,
                const G4String& hitsCollectionName);
    virtual ~TrackerSD();
  
    virtual void   Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);

    unsigned int GetDetectorID() const { return fDetectorID; };

    void SetDetectorID(const unsigned int id){ fDetectorID = id; };

  private:
    G4THitsCollection<TrackerHit>* fTrackerHitsCollection;
    unsigned int fDetectorID;
};
