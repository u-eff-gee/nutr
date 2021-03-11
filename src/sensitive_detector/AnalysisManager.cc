#include <ctime>

using std::time;

#include "AnalysisManager.hh"

AnalysisManager::AnalysisManager(const string out_file_name): fFactoryOn(false){
    
    if(out_file_name == ""){
        output_file_name = to_string(time(nullptr));
    } else {
        output_file_name = out_file_name;
    }
    
}

void AnalysisManager::Book(){

    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    // The command below merges the output created by different threads into a single file.
    // This does not work for some file formats.
    // Geant4 will print a warning during execution and refuse to merge the files.
    // In principle, one could use SetNtupleMerging only if OUTPUT_FORMAT="root".
    // However, it was chosen to keep the warning message here so that a user who has been working
    // with OUTPUT_FORMAT="root" and switches to OUTPUT_FORMAT="csv" will not wonder why the files
    // are not merged any more.
    analysisManager->SetNtupleMerging(true);
    analysisManager->OpenFile(output_file_name);
    CreateNtupleColumns(analysisManager);
    analysisManager->FinishNtuple();

    fFactoryOn = true;

}

void AnalysisManager::FillNtuple(G4int eventID, vector<shared_ptr<G4VHit>> hits){

    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    FillNtupleColumns(analysisManager, eventID, hits);
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
