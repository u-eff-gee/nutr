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

#include "G4DecayPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4EmLivermorePolarizedPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4SystemOfUnits.hh"

#include "Physics.hh"
#include "PhysicsConfig.hh"

Physics::Physics() {

  RegisterPhysics(new G4EmLivermorePolarizedPhysics());

  if constexpr (physics_build_options.use_em_extra_physics) {
    G4EmExtraPhysics *emExtraPhysics = new G4EmExtraPhysics();
    emExtraPhysics->LENDGammaNuclear(
        physics_build_options.use_lendgammanuclear);
    RegisterPhysics(emExtraPhysics);
  }

  if constexpr (physics_build_options.use_decay_physics) {
    RegisterPhysics(new G4DecayPhysics());
    RegisterPhysics(new G4RadioactiveDecayPhysics());
  }

  if constexpr (physics_build_options.use_hadron_physics) {
    RegisterPhysics(new HadronElastic());

    RegisterPhysics(new HadronInelastic());
  }
}

void Physics::SetCuts() {
  G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(
      physics_build_options.production_cut_low_keV * keV, 1. * GeV);
}
