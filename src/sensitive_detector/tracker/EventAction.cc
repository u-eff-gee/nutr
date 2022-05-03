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

    Copyright (C) 2020-2022 Udo Friman-Gayer
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
  G4int eventID = event->GetEventID();

  G4VHitsCollection *hc = nullptr;
  for (int n_hc = 0; n_hc < event->GetHCofThisEvent()->GetNumberOfCollections();
       ++n_hc) {
    hc = event->GetHCofThisEvent()->GetHC(n_hc);

    for (size_t i = 0; i < hc->GetSize(); ++i)
      analysis_manager->FillNtuple(
          eventID, {make_shared<DetectorHit>((DetectorHit *)hc->GetHit(i))});
  }
}