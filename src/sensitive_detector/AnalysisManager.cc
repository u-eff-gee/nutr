#include <ctime>
#include <filesystem>

using std::time;

#include "G4Threading.hh"

#include "AnalysisManager.hh"
#include "NutrMessenger.hh"
#include "SensitiveDetectorBuildOptions.hh"

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

  auto output_file_name_macro = NutrMessenger::GetFilename();
  if (output_file_name_macro != "") {
    output_file_name = output_file_name_macro;
  } else if (output_file_name == "") {
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

void AnalysisManager::CreateNtupleColumns(G4AnalysisManager *analysisManager) {

  analysisManager->CreateNtupleIColumn("evid");

  if constexpr (sensitive_detector_build_options.track_primary) {
    analysisManager->CreateNtupleDColumn("pos0x");
    analysisManager->CreateNtupleDColumn("pos0y");
    analysisManager->CreateNtupleDColumn("pos0z");
    analysisManager->CreateNtupleDColumn("mom0x");
    analysisManager->CreateNtupleDColumn("mom0y");
    analysisManager->CreateNtupleDColumn("mom0z");
  }
}

void AnalysisManager::FillNtuple(const G4Event *event,
                                 vector<shared_ptr<G4VHit>> &hits) {

  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  FillNtupleColumns(analysisManager, event, hits);
  analysisManager->AddNtupleRow(0);
}

size_t AnalysisManager::FillNtupleColumns(
    G4AnalysisManager *analysisManager, const G4Event *event,
    [[maybe_unused]] vector<shared_ptr<G4VHit>> &hits) {

  size_t col = 0;
  analysisManager->FillNtupleIColumn(0, col++, event->GetEventID());

  if constexpr (sensitive_detector_build_options.track_primary) {
    const G4PrimaryVertex *primary_vertex = event->GetPrimaryVertex(0);
    if (primary_vertex != nullptr) {
      analysisManager->FillNtupleDColumn(0, col++, primary_vertex->GetX0());
      analysisManager->FillNtupleDColumn(0, col++, primary_vertex->GetY0());
      analysisManager->FillNtupleDColumn(0, col++, primary_vertex->GetZ0());

      const G4PrimaryParticle *primary_particle = primary_vertex->GetPrimary();
      if (primary_particle != nullptr) {
        analysisManager->FillNtupleDColumn(0, col++, primary_particle->GetPx());
        analysisManager->FillNtupleDColumn(0, col++, primary_particle->GetPy());
        analysisManager->FillNtupleDColumn(0, col++, primary_particle->GetPz());
      } else {
        col += 3;
      }
    } else {
      col += 6;
    }
  }

  return col;
}

void AnalysisManager::Save() {

  if (!fFactoryOn)
    return;

  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();

  if (G4Threading::G4GetThreadId() == 0) {
    G4cout << "Created output file '" << analysisManager->GetFileName() << "'."
           << G4endl;
  }

  fFactoryOn = false;
}
