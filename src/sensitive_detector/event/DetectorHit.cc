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

#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<DetectorHit>* DetectorHitAllocator=0;

DetectorHit::DetectorHit()
 : NDetectorHit(),
   fEdep(0.)
{}

DetectorHit::DetectorHit(const DetectorHit& right)
  : NDetectorHit()
{
  fDetectorID = right.fDetectorID;
  fEdep      = right.fEdep;
}

const DetectorHit& DetectorHit::operator=(const DetectorHit& right)
{
  fDetectorID = right.fDetectorID;
  fEdep      = right.fEdep;

  return *this;
}