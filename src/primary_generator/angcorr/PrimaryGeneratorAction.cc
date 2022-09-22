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

#include <array>
#include <charconv>
#include <iostream>
#include <memory>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <system_error>
#include <utility>
#include <vector>

using std::array;
using std::make_unique;
using std::vector;

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "AngularCorrelation.hh"
#include "CascadeRejectionSampler.hh"
#include "NDetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorMessenger.hh"
#include "SourceVolume.hh"
#include "State.hh"

template <typename F>
void split_string_foreach(const std::string &str, const std::string &delim,
                          F &&func) {
  size_t start = 0;
  size_t end = str.find(delim);

  while (end != std::string::npos) {
    func(str.substr(start, end - start));
    start = end + delim.length();
    end = str.find(delim, start);
  }
  func(str.substr(start));
}

std::pair<std::vector<State>, std::vector<double>>
parse_cascade(const std::string &cascade) {
  std::vector<State> states;
  std::vector<double> deltas;
  double last_delta = 0.;
  bool is_first_state = true;

  split_string_foreach(cascade, " ", [&](std::string_view str) {
    str.remove_prefix(std::min(str.find_first_not_of(" "), str.size()));
    str.remove_suffix(
        std::min(str.size() - str.find_last_not_of(" ") - 1, str.size()));

    if (str.starts_with("[") && str.ends_with("]")) {
      str.remove_suffix(1);
      str.remove_prefix(1);

      last_delta = std::stod(std::string{str});
    } else {
      Parity par = parity_unknown;
      switch (str.back()) {
      case '+':
        par = positive;
        str.remove_suffix(1);
        break;
      case '-':
        par = negative;
        str.remove_suffix(1);
        break;
      default:
        break;
      }

      int two_J_factor = 2;
      if (str.ends_with("/2")) {
        str.remove_suffix(2);
        two_J_factor = 1;
      }
      int two_J = std::stoi(std::string{str});

      states.emplace_back(two_J * two_J_factor, par);
      if (!is_first_state) {
        deltas.push_back(last_delta);
      }
      is_first_state = false;
      last_delta = 0.;
    }
  });
  return {states, deltas};
}

Transition get_transition(const State s1, const State s2, double delta = 0.) {
  auto multipolarity = std::max(2, std::abs(s2.two_J - s1.two_J));
  if (s1.parity == parity_unknown || s2.parity == parity_unknown) {
    return Transition(multipolarity, multipolarity + 2, delta);
  } else if ((s1.parity != s2.parity) != !(multipolarity % 4)) {
    return Transition(electric, multipolarity, magnetic, multipolarity + 2,
                      delta);
  } else {
    return Transition(magnetic, multipolarity, electric, multipolarity + 2,
                      delta);
  }
}

std::vector<AngularCorrelation>
parse_angular_correlation(std::vector<State> states,
                          std::vector<double> deltas) {
  assert(states.size() >= 3);
  if (deltas.size() == 0) {
    deltas.resize(states.size() - 1, 0.);
  }
  assert(deltas.size() == states.size() - 1);

  std::vector<AngularCorrelation> cascade;
  cascade.reserve(states.size() - 1);

  for (size_t i = 0; i + 2 < states.size(); ++i) {
    cascade.emplace_back(AngularCorrelation{
        states[i],
        {{get_transition(states[i], states[i + 1], deltas[i]), states[i + 1]},
         {get_transition(states[i + 1], states[i + 2], deltas[i + 1]),
          states[i + 2]}}});
  }
  return cascade;
}

PrimaryGeneratorAction::PrimaryGeneratorAction(const long seed)
    : G4VUserPrimaryGeneratorAction(), cas_rej_sam(nullptr),
      random_number_seed(seed + G4Threading::G4GetThreadId()) {

  particle_gun = make_unique<G4ParticleGun>(1);
  particle_gun->SetParticleDefinition(
      G4ParticleTable::GetParticleTable()->FindParticle("gamma"));

  messenger = new PrimaryGeneratorMessenger(this);

  source_volumes = ((NDetectorConstruction *)G4RunManager::GetRunManager()
                        ->GetUserDetectorConstruction())
                       ->GetSourceVolumes();
  normalize_intensities();

  if (source_volumes.size() > 0) {
    random_engine = mt19937(random_number_seed +
                            2 * G4Threading::GetNumberOfRunningWorkerThreads());
    for (size_t i = 0; i < source_volumes.size(); ++i) {
      source_volumes[i]->initialize(
          random_number_seed +
          (10 + i) * G4Threading::GetNumberOfRunningWorkerThreads());
    }
  }
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *event) {

  if (cascade_energies.size() != cascade.size() ||
      cascade_energies.size() == 0) {
    std::cerr << "Alpaca not initialized. Use /alpaca/cascade and "
                 "/alpaca/energies.\n";
    std::cerr << "cascade_energies: " << cascade_energies.size() << "\n";
    std::cerr << "cascade: " << cascade.size() << "\n";
    event->SetEventAborted();
    return;
  }

  const double ran_uni = uniform_random(random_engine);
  for (size_t i = 0; i < source_volumes.size(); ++i) {
    if (ran_uni <= relative_intensities_normalized[i]) {
      auto position = source_volumes[i]->operator()();
      particle_gun->SetParticlePosition(position);
      break;
    }
  }

  vector<array<double, 2>> transitions_theta_phi = cas_rej_sam->operator()();

  double sine_theta;
  for (size_t n_transition = 0; n_transition < transitions_theta_phi.size();
       ++n_transition) {
    if (cascade_energies[n_transition] > 0.) {
      auto [theta, phi] = transitions_theta_phi[n_transition];
      sine_theta = sin(theta);

      particle_gun->SetParticleMomentumDirection(G4ThreeVector(
          sine_theta * cos(phi), sine_theta * sin(phi), cos(theta)));

      particle_gun->SetParticleEnergy(cascade_energies[n_transition]);
      particle_gun->GeneratePrimaryVertex(event);
    }
  }
}

void PrimaryGeneratorAction::normalize_intensities() {

  double unnormalized_intensities_sum = 0.;
  for (size_t i = 0; i < source_volumes.size(); ++i) {
    unnormalized_intensities_sum += source_volumes[i]->get_relative_intensity();
    relative_intensities_normalized.push_back(unnormalized_intensities_sum);
  }

  double norm_factor = 1. / unnormalized_intensities_sum;

  for (size_t i = 0; i < source_volumes.size(); ++i) {
    relative_intensities_normalized[i] =
        norm_factor * relative_intensities_normalized[i];
  }
}

void PrimaryGeneratorAction::set_energies(const std::string &s_energies) {
  const std::string delim = " ";
  size_t start = 0;
  size_t end = s_energies.find(delim);

  cascade_energies.clear();
  while (end != std::string::npos) {
    const auto str = s_energies.substr(start, end - start);
    start = end + delim.length();
    end = s_energies.find(delim, start);

    cascade_energies.push_back(std::stod(str));
  }

  auto unit = s_energies.substr(start, end - start);
  if (G4Threading::G4GetThreadId() == 0) {
    std::stringstream ss;
    ss << "Set alpaca energies to";
    for (auto energy : cascade_energies) {
      ss << " " << energy << " " << unit << ",";
    }
    ss.seekp(-1, ss.cur);
    ss << ".\n";
    std::cout << ss.str();
  }

  double k = G4UnitDefinition::GetValueOf(unit);
  std::transform(cascade_energies.cbegin(), cascade_energies.cend(),
                 cascade_energies.begin(), [k](double c) { return c * k; });
}

void PrimaryGeneratorAction::set_cascade(const std::string &s_cascade) {
  auto [states, deltas] = parse_cascade(s_cascade);
  cascade = parse_angular_correlation(states, deltas);

  if (G4Threading::G4GetThreadId() == 0) {
    std::stringstream ss;
    ss << "Set alpaca cascade to";
    for (auto &state : states) {
      ss << " " << state.str_rep() << " ->";
    }
    ss.seekp(-3, ss.cur);
    ss << ", with deltas";
    for (auto delta : deltas) {
      ss << " " << delta << ",";
    }
    ss.seekp(-1, ss.cur);
    ss << ".\n";
    std::cout << ss.str();
  }

  cas_rej_sam = unique_ptr<CascadeRejectionSampler>(new CascadeRejectionSampler(
      cascade,
      random_number_seed + 3 * G4Threading::GetNumberOfRunningWorkerThreads(),
      {0., 0., 0.}, false));
}

void PrimaryGeneratorAction::set_particle(const std::string &particle) {
  particle_gun->SetParticleDefinition(
      G4ParticleTable::GetParticleTable()->FindParticle(particle));
}
