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

#include "G4SDManager.hh"

#include "NDetectorConstruction.hh"
#include "SensitiveDetector.hh"

void NDetectorConstruction::RegisterSensitiveLogicalVolumes(vector<G4LogicalVolume*> logical_volumes){
    for(auto log_vol: logical_volumes){
        sensitive_logical_volumes.push_back(log_vol);
    }
}

void NDetectorConstruction::ConstructSDandField(){
    
    SensitiveDetector* sen_det = nullptr;

    for(size_t i = 0; i < sensitive_logical_volumes.size(); ++i){
        sen_det = new SensitiveDetector(sensitive_logical_volumes[i]->GetName(), sensitive_logical_volumes[i]->GetName());
        sen_det->SetDetectorID(i);
        G4SDManager::GetSDMpointer()->AddNewDetector(sen_det);
        SetSensitiveDetector(sensitive_logical_volumes[i]->GetName(), sen_det, true);
    }

}