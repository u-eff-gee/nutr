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

#include <ctime>

using std::localtime;
using std::time;

#include <iomanip>

using std::put_time;

#include "NRunAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4Threading.hh"

NRunAction::NRunAction(AnalysisManager* ana_man)
 : G4UserRunAction(), analysis_manager(ana_man), start_time(system_clock::now())
{}

void NRunAction::BeginOfRunAction(const G4Run*)
{
    const time_t start_time_t = system_clock::to_time_t(start_time);
    G4cout << "Run started on " << put_time(localtime(&start_time_t), "%F %T (thread ID ") << G4Threading::G4GetThreadId() << ")" << G4endl;
	analysis_manager->Book();
}

void NRunAction::EndOfRunAction(const G4Run* )
{
	analysis_manager->Save();
}
