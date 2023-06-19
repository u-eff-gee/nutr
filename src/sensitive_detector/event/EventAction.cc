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

#include <algorithm>
#include <memory>

using std::make_unique;
using std::unique_ptr;

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4ios.hh"

#include "DetectorHit.hh"
#include "EventAction.hh"
#include "SensitiveDetectorBuildOptions.hh"

EventAction::EventAction(AnalysisManager *ana_man) : NEventAction(ana_man) {}

void EventAction::EndOfEventAction(const G4Event *event) {
  G4VHitsCollection *hc = nullptr;

  vector<unique_ptr<DetectorHit>> hits_owned;
  hits_owned.emplace_back();

  int current_deid{0};
  int max_deid{0};
  double sum_edep = 0.;

  for (int n_hc = 0; n_hc < event->GetHCofThisEvent()->GetNumberOfCollections();
       ++n_hc) {

    hc = event->GetHCofThisEvent()->GetHC(n_hc);

    if (hc->GetSize() > 0) {
      current_deid = ((DetectorHit *)hc->GetHit(0))->GetDetectorID();

      // Fill the list of hits up to the current detector ID, if it is larger
      // than the current maximum.
      if (current_deid > max_deid) {
        for (int i = 0; i < current_deid - max_deid; ++i) {
          hits_owned.emplace_back();
        }
        max_deid = current_deid;
      }

      double edep = 0.;
      for (size_t i = 0; i < hc->GetSize(); ++i) {
        edep += ((DetectorHit *)hc->GetHit(i))->GetEdep();
      }

      sum_edep += edep;
      hits_owned[current_deid]->SetEdep(edep);
    }
  }

  if (sensitive_detector_build_options.track_primary || sum_edep > 0.) {
    vector<G4VHit *> hits_raw;
    std::transform(hits_owned.begin(), hits_owned.end(),
                   std::back_inserter(hits_raw),
                   [](const std::unique_ptr<DetectorHit> &ptr) {
                     return static_cast<G4VHit *>(ptr.get());
                   });
    analysis_manager->FillNtuple(event, hits_raw);
  }
}
