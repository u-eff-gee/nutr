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

#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

#include "NDetectorHit.hh"

class DetectorHit : public NDetectorHit
{
  public:
    DetectorHit();
    DetectorHit(const DetectorHit&);

    const DetectorHit& operator=(const DetectorHit&);

    void SetDetectorID (const G4int detectorID) { fDetectorID = detectorID; };
    void SetEdep     (const G4double de) { fEdep = de; };
    void SetPos      (const G4ThreeVector xyz) { fPos = xyz; };

    G4int GetDetectorID() const { return fDetectorID; };
    G4double GetEdep() const { return fEdep; };
    G4ThreeVector GetPos() const { return fPos; };

  private:
      G4int         fDetectorID;
      G4double      fEdep;
      G4ThreeVector fPos;
};