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

#include "NRunAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"

NRunAction::NRunAction(AnalysisManager* ana_man)
 : G4UserRunAction(), analysis_manager(ana_man)
{}

void NRunAction::BeginOfRunAction(const G4Run*)
{ 
	analysis_manager->Book();
}

void NRunAction::EndOfRunAction(const G4Run* )
{
	analysis_manager->Save();
}
