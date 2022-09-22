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

    Copyright (C) 2020-2022 Udo Friman-Gayer and Oliver Papst
*/

#pragma once

#include <functional>
#include <memory>
#include <random>
#include <string_view>
#include <vector>

using std::shared_ptr;
using std::uniform_real_distribution;
using std::unique_ptr;
using std::vector;

#include "G4VUserPrimaryGeneratorAction.hh"
#include "PrimaryGeneratorMessenger.hh"

class PrimaryGeneratorMessenger;
class G4ParticleGun;
class SourceVolume;
class CascadeRejectionSampler;
class AngularCorrelation;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
  PrimaryGeneratorAction(long seed);

  void GeneratePrimaries(G4Event *) override final;

  void set_cascade(const std::string &);
  void set_energies(const std::string &);
  void set_particle(const std::string &);
  void set_force_point_source(bool);

private:
  void normalize_intensities();

  unique_ptr<G4ParticleGun> particle_gun;
  unique_ptr<CascadeRejectionSampler> cas_rej_sam;
  vector<double> cascade_energies;
  vector<AngularCorrelation> cascade;
  bool force_point_source;

  vector<shared_ptr<SourceVolume>> source_volumes;
  vector<double> relative_intensities_normalized;

  PrimaryGeneratorMessenger messenger;

  long random_number_seed;
  std::mt19937 random_engine; /**< Deterministic random number engine. */
  uniform_real_distribution<double>
      uniform_random; /**< Uniform distribution from which all random numbers
                         are derived here. */
};
