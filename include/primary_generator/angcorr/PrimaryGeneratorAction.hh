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

#pragma once

#include <functional>
#include <memory>
#include <random>

using std::mt19937;
using std::shared_ptr;
using std::uniform_real_distribution;
using std::unique_ptr;

#include "G4ParticleGun.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

#include "AngCorrRejectionSampler.hh"
#include "SphereRejectionSampler.hh"
#include "SourceVolume.hh"

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    PrimaryGeneratorAction();

    void GeneratePrimaries(G4Event*) override final;

private:
    void normalize_intensities();

    unique_ptr<G4ParticleGun> particle_gun;
	unique_ptr<AngCorrRejectionSampler> sph_rej_sam;

    vector<shared_ptr<SourceVolume>> source_volumes;
    vector<double> relative_intensities_normalized;

    mt19937 random_engine; /**< Deterministic random number engine. */
    uniform_real_distribution<double> uniform_random; /**< Uniform distribution from which all random numbers are derived here. */
};