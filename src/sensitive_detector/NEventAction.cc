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

#include <chrono>

using std::chrono::duration;
using std::chrono::system_clock;
using std::chrono::time_point;

#include <ctime>

using std::localtime;

#include <iomanip>

using std::put_time;
using std::scientific;
using std::setprecision;
using std::setw;

#include "G4RunManager.hh"

#include "NEventAction.hh"
#include "NRunAction.hh"

void NEventAction::BeginOfEventAction(const G4Event* event){

    const int eventID = event->GetEventID();

    if(eventID % update_frequency == 0){
        NRunAction *run_action = (NRunAction*) G4RunManager::GetRunManager()->GetUserRunAction();

        const time_point<system_clock> current_time = system_clock::now();
        const time_t current_time_t = system_clock::to_time_t(current_time);
        duration<double> delta_t = current_time - run_action->get_start_time();
        G4cout << put_time(localtime(&current_time_t), "%F %T") << " ( " << scientific << setprecision(8) << delta_t.count() << " s since start ) : Event #" << setw(10) << eventID << G4endl;
    }
}
