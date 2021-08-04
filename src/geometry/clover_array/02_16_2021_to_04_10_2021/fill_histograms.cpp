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

#include <array>

using std::array;

#include <iostream>

using std::cout;
using std::endl;

#include <string>

using std::string;
using std::to_string;

#include <vector>

using std::vector;

#include <utility>

using std::pair;

#include "TChain.h"
#include "TFile.h"
#include "TH1.h"

struct Detector{
    const string name;
    const unsigned int n_channels;
};

vector<Detector> detectors{
    Detector{"clover_1", 4},
    Detector{"clover_3", 4},
    Detector{"clover_5", 4},
    Detector{"clover_7", 4},
    Detector{"clover_B1", 4},
    Detector{"clover_B4", 4},
    Detector{"clover_B5", 4},
    Detector{"clover_B6", 4},

    Detector{"cebr_B", 1},
    Detector{"cebr_D", 1},
    Detector{"cebr_F", 1},
    Detector{"cebr_H", 1},
    Detector{"cebr_K", 1},
    Detector{"cebr_O", 1},
    Detector{"cebr_BB", 1},
    Detector{"cebr_BC", 1},
    Detector{"cebr_BD", 1},
    Detector{"cebr_BK", 1},
    Detector{"cebr_BL", 1},
    Detector{"cebr_BM", 1},
};

int main(int argc, char **argv){

    TChain* tree = new TChain("edep");

    // Read simulation output files.
    for(int i = 1; i < argc-1; ++i){
        cout << "Reading ROOT file '" << argv[i] << "' ..." << endl;
        tree->Add(argv[i]);
    }

    // Create histograms.
    const int nbins = 16384;
    const double xlow = -0.0005;
    const double xup = 16.3835;

    vector<TH1D> channel_spectra;
    vector<TH1D> sum_spectra;
    vector<TH1D> addback_spectra;

    size_t n_channels = 0;

    for(auto detector: detectors){
        if(detector.n_channels > 1){
            for(unsigned int n_channel = 0; n_channel < detector.n_channels; ++n_channel){
                channel_spectra.push_back(
                    TH1D((detector.name + "_E" + to_string(n_channel+1)).c_str(), (detector.name + "_E" + to_string(n_channel+1)).c_str(), nbins, xlow, xup)
                );
                ++n_channels;
            }
            sum_spectra.push_back(
                TH1D((detector.name + "_sum").c_str(), (detector.name + "_sum").c_str(), nbins, xlow, xup)
            );
            addback_spectra.push_back(
                TH1D((detector.name + "_addback").c_str(), (detector.name + "_addback").c_str(), nbins, xlow, xup)
            );
        } else {
            channel_spectra.push_back(
                TH1D(detector.name.c_str(), detector.name.c_str(), nbins, xlow, xup)
            );
            ++n_channels;
        }
    }

    // Link branches.
    size_t channel_index = 0;
    vector<double> energy_deposition(n_channels, 0.);
    for(auto detector: detectors){
        for(unsigned int n_channel = 0; n_channel < detector.n_channels; ++n_channel){
            tree->SetBranchAddress(("det" + to_string(channel_index)).c_str(), &energy_deposition[channel_index]);
            ++channel_index;
        }
    }

    // Fill histograms.
    double addback = 0;

    for(int n_event = 1; n_event <= tree->GetEntries(); ++n_event){
        tree->GetEntry(n_event);
        channel_index = 0;
        for(size_t n_detector = 0; n_detector < detectors.size(); ++n_detector){
            if(detectors[n_detector].n_channels > 1){
                addback = 0;
                for(unsigned int n_channel = 0; n_channel < detectors[n_detector].n_channels; ++n_channel){            
                    if(energy_deposition[channel_index] > 0.){
                        channel_spectra[channel_index].Fill(energy_deposition[channel_index]);
                        sum_spectra[n_detector].Fill(energy_deposition[channel_index]);
                        addback += energy_deposition[channel_index];
                    }
                    ++channel_index;
                }
                if(addback > 0.){
                    addback_spectra[n_detector].Fill(addback);
                }
            } else {
                if(energy_deposition[channel_index] > 0.){
                    channel_spectra[channel_index].Fill(energy_deposition[channel_index]);
                }
                ++channel_index;
            }
        }
    }

    // Write histograms to output file.
    cout << "Writing histograms to '" << argv[argc-1] << "' ..." << endl;
    TFile* output_file = new TFile(argv[argc-1], "RECREATE");
    for(auto histogram: channel_spectra)
        histogram.Write();
    for(auto histogram: sum_spectra)
        histogram.Write();
    for(auto histogram: addback_spectra)
        histogram.Write();

    output_file->Close();
}