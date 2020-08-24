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

#pragma once

#include <chrono>

using std::chrono::system_clock;
using std::chrono::system_clock;
using std::chrono::time_point;

#include "G4UserRunAction.hh"
#include "globals.hh"

#include "AnalysisManager.hh"

class NRunAction : public G4UserRunAction
{
public:
    NRunAction(AnalysisManager* ana_man);

    void BeginOfRunAction(const G4Run* run) override;
    void EndOfRunAction(const G4Run* run) override;

    time_point<system_clock> get_start_time(){ return start_time; };

private:
    AnalysisManager* analysis_manager;
    const time_point<system_clock> start_time;
};
