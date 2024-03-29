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

    Copyright (C) 2020-2022 Udo Friman-Gayer and Oliver Papst
*/

#include "DetectorHit.hh"

G4ThreadLocal G4Allocator<DetectorHit> *DetectorHitAllocator = 0;

DetectorHit::DetectorHit()
    : NDetectorHit(), fTrackID(-1), fParticleID(0), fGlobalTime(0.), fEdep(0.),
      fEkin(0.), fPos(G4ThreeVector()), fMom(G4ThreeVector()) {}

DetectorHit::DetectorHit(const DetectorHit &right) : NDetectorHit() {
  fTrackID = right.fTrackID;
  fParticleID = right.fParticleID;
  fDetectorID = right.fDetectorID;
  fGlobalTime = right.fGlobalTime;
  fEdep = right.fEdep;
  fEkin = right.fEkin;
  fPos = right.fPos;
  fMom = right.fMom;
}

DetectorHit::DetectorHit(DetectorHit *right) : NDetectorHit() {
  fTrackID = right->fTrackID;
  fParticleID = right->fParticleID;
  fDetectorID = right->fDetectorID;
  fGlobalTime = right->fGlobalTime;
  fEdep = right->fEdep;
  fEkin = right->fEkin;
  fPos = right->fPos;
  fMom = right->fMom;
}

const DetectorHit &DetectorHit::operator=(const DetectorHit &right) {
  fTrackID = right.fTrackID;
  fParticleID = right.fParticleID;
  fDetectorID = right.fDetectorID;
  fGlobalTime = right.fGlobalTime;
  fEkin = right.fEkin;
  fEdep = right.fEdep;
  fPos = right.fPos;
  fMom = right.fMom;

  return *this;
}