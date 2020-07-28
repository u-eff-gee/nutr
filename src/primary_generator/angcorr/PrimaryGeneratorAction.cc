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

#include <utility>

using std::pair;

#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"

#include "PrimaryGeneratorAction.hh"
#include "W_pol_dir.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(), particle_gun(new G4ParticleGun(1))
{
    particle_gun->SetParticleDefinition(G4ParticleTable::GetParticleTable()->FindParticle("gamma"));
    particle_gun->SetParticleEnergy(1.*MeV);
    particle_gun->SetParticlePosition(G4ThreeVector());

    sph_rej_sam = std::make_unique<AngCorrRejectionSampler>(
        new W_pol_dir(
            State(0, positive),
            {
                {Transition(magnetic, 2, electric, 4, 0.), State(2, positive)},
                {Transition(em_unknown, 2, em_unknown, 4, 0.), State(0, parity_unknown)}
            }
        ), 0
    );

}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
    pair<double, double> theta_phi = sph_rej_sam->operator()();
    double sine_theta = sin(theta_phi.first);

    particle_gun->SetParticleMomentumDirection(
        G4ThreeVector(
            sine_theta*cos(theta_phi.second),
            sine_theta*sin(theta_phi.second),
            cos(theta_phi.first)
        )
    );

    particle_gun->GeneratePrimaryVertex(event);
}