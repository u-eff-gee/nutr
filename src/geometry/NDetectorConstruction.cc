#include "G4SDManager.hh"

#include "NDetectorConstruction.hh"
#include "SensitiveDetector.hh"

NDetectorConstruction::NDetectorConstruction()
:G4VUserDetectorConstruction()
{}

NDetectorConstruction::~NDetectorConstruction()
{}

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