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

#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<DetectorHit>* DetectorHitAllocator=0;

DetectorHit::DetectorHit()
 : G4VHit(),
   fTrackID(-1),
   fParticleID(0),
   fDetectorID(0),
   fEdep(0.),
   fPos(G4ThreeVector())
{}

DetectorHit::~DetectorHit() {}

DetectorHit::DetectorHit(const DetectorHit& right)
  : G4VHit()
{
  fTrackID   = right.fTrackID;
  fParticleID = right.fParticleID;
  fDetectorID = right.fDetectorID;
  fEdep      = right.fEdep;
  fPos       = right.fPos;
}

const DetectorHit& DetectorHit::operator=(const DetectorHit& right)
{
  fTrackID   = right.fTrackID;
  fParticleID = right.fParticleID;
  fDetectorID = right.fDetectorID;
  fEdep      = right.fEdep;
  fPos       = right.fPos;

  return *this;
}

G4bool DetectorHit::operator==(const DetectorHit& right) const
{
  return ( this == &right ) ? true : false;
}