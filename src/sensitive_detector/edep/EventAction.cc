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
using std::shared_ptr;

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4ios.hh"

#include "DetectorHit.hh"
#include "EventAction.hh"

EventAction::EventAction(AnalysisManager *ana_man) : NEventAction(ana_man) {}

void EventAction::EndOfEventAction(const G4Event *event) {
  G4VHitsCollection *hc = nullptr;
  shared_ptr<DetectorHit> cumulative_hit;

  for (int n_hc = 0; n_hc < event->GetHCofThisEvent()->GetNumberOfCollections();
       ++n_hc) {

    hc = event->GetHCofThisEvent()->GetHC(n_hc);

    if (hc->GetSize() > 0) {
      double edep = 0.;
      for (size_t i = 0; i < hc->GetSize(); ++i)
        edep += ((DetectorHit *)hc->GetHit(i))->GetEdep();

      cumulative_hit = make_shared<DetectorHit>();
      cumulative_hit->SetDetectorID(
          ((DetectorHit *)hc->GetHit(0))->GetDetectorID());
      cumulative_hit->SetEdep(edep);
      vector<shared_ptr<G4VHit>> hits{cumulative_hit}; // Wrap cumulative hit into a vector for compatibility with the AnalysisManager API.
      analysis_manager->FillNtuple(event, hits);
    }
  }
}
