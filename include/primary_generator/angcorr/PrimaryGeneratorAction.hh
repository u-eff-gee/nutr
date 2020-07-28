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

using std::unique_ptr;

#include "G4ParticleGun.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

#include "AngCorrRejectionSampler.hh"
#include "SphereRejectionSampler.hh"

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    PrimaryGeneratorAction();

    void GeneratePrimaries(G4Event*) override final;

private:
    unique_ptr<G4ParticleGun> particle_gun;
	unique_ptr<AngCorrRejectionSampler> sph_rej_sam;
};