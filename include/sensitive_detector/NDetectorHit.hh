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

#pragma once

#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"
#include "G4VHit.hh"
#include "tls.hh"

class NDetectorHit : public G4VHit {
public:
  NDetectorHit();
  NDetectorHit(const NDetectorHit &);

  const NDetectorHit &operator=(const NDetectorHit &);
  G4bool operator==(const NDetectorHit &) const;

  inline void *operator new(size_t);
  inline void operator delete(void *);

  void Draw(){};

  void SetDetectorID(const G4int detectorID) { fDetectorID = detectorID; };

  int GetDetectorID() const { return fDetectorID; };

protected:
  int fDetectorID;
};

extern G4ThreadLocal G4Allocator<NDetectorHit> *NDetectorHitAllocator;

inline void *NDetectorHit::operator new(size_t) {
  if (!NDetectorHitAllocator)
    NDetectorHitAllocator = new G4Allocator<NDetectorHit>;
  return (void *)NDetectorHitAllocator->MallocSingle();
}

inline void NDetectorHit::operator delete(void *hit) {
  NDetectorHitAllocator->FreeSingle((NDetectorHit *)hit);
}