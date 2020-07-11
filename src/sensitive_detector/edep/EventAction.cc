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

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"

#include "EventAction.hh"
#include "DetectorHit.hh"

EventAction::EventAction(TupleManager* tuple)
: G4UserEventAction(), fTupleManager(tuple)
{}

EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction(const G4Event*)
{}

void EventAction::EndOfEventAction(const G4Event* event)
{
	G4TrajectoryContainer* trajectoryContainer = event->GetTrajectoryContainer();
	G4int n_trajectories = 0;
	if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();

    for(size_t n_hc = 0; n_hc < event->GetHCofThisEvent()->GetNumberOfCollections(); ++n_hc){
	    
        G4VHitsCollection* hc = event->GetHCofThisEvent()->GetHC(n_hc);

        if(hc->GetSize() > 0){
            double edep = 0.;
            for(size_t i = 0; i < hc->GetSize(); ++i)
                edep += ((DetectorHit*) hc->GetHit(i))->GetEdep();

            fTupleManager->FillNtuple(((DetectorHit*) hc->GetHit(0))->GetDetectorID(), edep);
        }
    }
}