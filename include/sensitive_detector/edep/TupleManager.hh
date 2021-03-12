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

    Copyright (C) 2020, 2021 Udo Friman-Gayer
*/

#pragma once

#include "AnalysisManager.hh"

class TupleManager : public AnalysisManager
{
public:
    TupleManager(const string out_file_name): AnalysisManager(out_file_name){};

    void CreateNtupleColumns(G4VAnalysisManager* analysisManager) override;

    void FillNtupleColumns(G4VAnalysisManager* analysisManager, G4int eventID, vector<shared_ptr<G4VHit>> hits) override;};
