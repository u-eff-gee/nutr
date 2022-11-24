#include <ctime>
#include <filesystem>

using std::time;

#include "AnalysisManager.hh"

AnalysisManager::AnalysisManager() : fFactoryOn(false) {}

string AnalysisManager::create_default_file_name() const {
  string prefix = to_string(time(nullptr));
  string file_name_proposal = prefix + ".root";
  if (std::filesystem::exists(file_name_proposal)) {
    unsigned int i = 0;
    while (true) {
      file_name_proposal = prefix + "_" + to_string(i) + ".root";
      if (!std::filesystem::exists(file_name_proposal)) {
        break;
      }
      ++i;
    }
  }

  return file_name_proposal;
}

void AnalysisManager::Book(string output_file_name) {

  if (output_file_name == "") {
    output_file_name = create_default_file_name();
  }

  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  // The command below merges the output created by different threads into a
  // single file. This does not work for some file formats. Geant4 will print a
  // warning during execution and refuse to merge the files. In principle, one
  // could use SetNtupleMerging only if OUTPUT_FORMAT="root". However, it was
  // chosen to keep the warning message here so that a user who has been working
  // with OUTPUT_FORMAT="root" and switches to OUTPUT_FORMAT="csv" will not
  // wonder why the files are not merged any more.
  analysisManager->SetNtupleMerging(true);
  analysisManager->OpenFile(output_file_name);
  CreateNtupleColumns(analysisManager);
  analysisManager->FinishNtuple();

  fFactoryOn = true;
}

void AnalysisManager::FillNtuple(int eventID, vector<shared_ptr<G4VHit>> hits,
                                 G4PrimaryVertex *primary_vertex) {

  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  FillNtupleColumns(analysisManager, eventID, hits, primary_vertex);
  analysisManager->AddNtupleRow(0);
}

void AnalysisManager::Save() {

  if (!fFactoryOn)
    return;

  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();

  G4cout << "Created output file '" << analysisManager->GetFileName() << "'."
         << G4endl;

  fFactoryOn = false;
}
