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
#include "tls.hh"

#include "NDetectorHit.hh"

class DetectorHit : public NDetectorHit {
public:
  DetectorHit();
  DetectorHit(const DetectorHit &);
  DetectorHit(DetectorHit *);

  const DetectorHit &operator=(const DetectorHit &);

  inline void *operator new(size_t);
  inline void operator delete(void *);

  void SetTrackID(const int track) { fTrackID = track; };
  void SetParticleID(const int pid) { fParticleID = pid; };
  void SetEdep(const double de) { fEdep = de; };
  void SetGlobalTime(const double time) { fGlobalTime = time; };
  void SetEnergy(const double ene) { fEkin = ene; };
  void SetPos(const G4ThreeVector xyz) { fPos = xyz; };
  void SetMom(const G4ThreeVector pxyz) { fMom = pxyz; };

  int GetTrackID() const { return fTrackID; };
  int GetParticleID() const { return fParticleID; };
  double GetGlobalTime() const { return fGlobalTime; };
  double GetEdep() const { return fEdep; };
  double GetEkin() const { return fEkin; };
  G4ThreeVector GetPos() const { return fPos; };
  G4ThreeVector GetMom() const { return fMom; };

private:
  int fTrackID;
  int fParticleID;
  double fGlobalTime;
  double fEdep;
  double fEkin;
  G4ThreeVector fPos;
  G4ThreeVector fMom;
};

extern G4ThreadLocal G4Allocator<DetectorHit> *DetectorHitAllocator;

inline void *DetectorHit::operator new(size_t) {
  if (!DetectorHitAllocator)
    DetectorHitAllocator = new G4Allocator<DetectorHit>;
  return (void *)DetectorHitAllocator->MallocSingle();
}

inline void DetectorHit::operator delete(void *hit) {
  DetectorHitAllocator->FreeSingle((DetectorHit *)hit);
}
