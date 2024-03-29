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

#include <memory>

using std::make_shared;

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4ios.hh"

#include "DetectorHit.hh"
#include "EventAction.hh"

EventAction::EventAction(AnalysisManager *ana_man) : NEventAction(ana_man) {}

void EventAction::EndOfEventAction(const G4Event *event) {
  G4VHitsCollection *hc = nullptr;
  int particleID{0}, trackID{0};
  DetectorHit *hit;

  for (int n_hc = 0; n_hc < event->GetHCofThisEvent()->GetNumberOfCollections();
       ++n_hc) {
    hc = event->GetHCofThisEvent()->GetHC(n_hc);

    if (hc->GetSize() > 0) {
      hit = (DetectorHit *)hc->GetHit(0);
      analysis_manager->FillNtuple(event, {hit});
      particleID = hit->GetParticleID();
      trackID = hit->GetTrackID();

      for (size_t i = 1; i < hc->GetSize(); ++i) {
        hit = (DetectorHit *)hc->GetHit(i);
        if (hit->GetParticleID() != particleID ||
            hit->GetTrackID() != trackID) {
          analysis_manager->FillNtuple(event, {hit});
          particleID = hit->GetParticleID();
          trackID = hit->GetTrackID();
        }
      }
    }
  }
}
