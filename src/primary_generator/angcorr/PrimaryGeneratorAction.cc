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

#include <memory>

using std::make_unique;

#include <vector>

using std::vector;

#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

#include "AngularCorrelation.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction(const long seed)
 : G4VUserPrimaryGeneratorAction(),
cas_rej_sam(nullptr), random_number_seed(seed)
{
    vector<AngularCorrelation> cascade{
        AngularCorrelation(
            State(0, positive),
            {
                {Transition(magnetic, 2, electric, 4, 0.), State(2, positive)},
                {Transition(em_unknown, 2, em_unknown, 4, 0.), State(0, parity_unknown)}
            }
        ),
    };
    cas_rej_sam = unique_ptr<CascadeRejectionSampler>(new CascadeRejectionSampler(cascade, random_number_seed, {0., 0., 0.}, false));

    particle_gun = make_unique<G4ParticleGun>(1);
    particle_gun->SetParticleDefinition(G4ParticleTable::GetParticleTable()->FindParticle("gamma"));
    particle_gun->SetParticleEnergy(1.*MeV);

    source_volumes = ((DetectorConstruction*) G4RunManager::GetRunManager()->GetUserDetectorConstruction())->GetSourceVolumes();
    normalize_intensities();

    if(source_volumes.size() > 0){
        random_engine = mt19937(random_number_seed);
        for(size_t i = 0; i < source_volumes.size(); ++i){
            source_volumes[i]->initialize(random_number_seed);
        }
    }
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{

    const double ran_uni = uniform_random(random_engine);
    for(size_t i = 0; i < source_volumes.size(); ++i){
        if(ran_uni <= relative_intensities_normalized[i]){
            particle_gun->SetParticlePosition(source_volumes[i]->operator()());
            break;
        }
    }
    vector<array<double, 2>> transitions_theta_phi = cas_rej_sam->operator()();

    double sine_theta;
    for(size_t n_transition = 0; n_transition < transitions_theta_phi.size(); ++n_transition){
        sine_theta = sin(transitions_theta_phi[n_transition][0]);

        particle_gun->SetParticleMomentumDirection(
            G4ThreeVector(
                sine_theta*cos(transitions_theta_phi[n_transition][1]),
                sine_theta*sin(transitions_theta_phi[n_transition][1]),
                cos(transitions_theta_phi[n_transition][0])
            )
        );

        particle_gun->GeneratePrimaryVertex(event);
    }
}

void PrimaryGeneratorAction::normalize_intensities(){

    double unnormalized_intensities_sum = 0.;
    for(size_t i = 0; i < source_volumes.size(); ++i){
        unnormalized_intensities_sum += source_volumes[i]->get_relative_intensity();
        relative_intensities_normalized.push_back(unnormalized_intensities_sum);
    }

    double norm_factor = 1./unnormalized_intensities_sum;

    for(size_t i = 0; i < source_volumes.size(); ++i){
        relative_intensities_normalized[i] = norm_factor*relative_intensities_normalized[i];
    }

}