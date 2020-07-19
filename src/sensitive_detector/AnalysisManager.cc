#include <ctime>

using std::time;

#include "AnalysisManager.hh"

AnalysisManager::AnalysisManager(const string out_file_name): fFactoryOn(false){
    
    if(out_file_name == ""){
        output_file_name = to_string(time(nullptr));
    } else {
        output_file_name = out_file_name;
    }
    
};

void AnalysisManager::Book(){

    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetNtupleMerging(true);
    G4bool fileOpen = analysisManager->OpenFile(output_file_name);
    CreateNtupleColumns(analysisManager);
    analysisManager->FinishNtuple();

    fFactoryOn = true;

}

void AnalysisManager::FillNtuple(G4int eventID, G4VHit* hit){

    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    FillNtupleColumns(analysisManager, eventID, hit);
    analysisManager->AddNtupleRow(0);

}


void AnalysisManager::Save(){

    if (! fFactoryOn) return;

    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();

    G4cout << "Created output file '" << analysisManager->GetFileName() << "'." << G4endl;

    delete G4AnalysisManager::Instance();
    fFactoryOn = false;

}
