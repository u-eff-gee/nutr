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

#include <G4ParticleGun.hh>
#include <G4ParticleTable.hh>
#include <PrimaryGeneratorAction.hh>
#include <PrimaryGeneratorMessenger.hh>

PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(
    PrimaryGeneratorAction *a_action)
    : action(a_action) {
  dir = new G4UIdirectory("/alpaca/");
  dir->SetGuidance("Settings specific to the angular correlation simulation");

  cmd_cascade = new G4UIcmdWithAString("/alpaca/cascade", this);
  cmd_cascade->SetGuidance(
      "Quantum numbers of states participating in simulated cascade.");
  cmd_cascade->SetGuidance("A list of values is expected.");
  cmd_cascade->SetGuidance("Non-zero multipole mixing ratios can be given in "
                           "square brackets inbetween states.");
  cmd_cascade->SetGuidance("The first transition corresponds to the excitation "
                           "process and is not observed (no energy assigned).");
  cmd_cascade->SetGuidance("Examples: 0+ 1- [0.1] 2 0");
  cmd_cascade->SetGuidance("          3/2- [-0.8] 5/2+ [0.8] 3/2");
  cmd_cascade->SetParameterName("cascade", false);

  cmd_energies = new G4UIcmdWithAString("/alpaca/energies", this);
  cmd_energies->SetGuidance(
      "Gamma-ray energies of transitions in simulated cascade.");
  cmd_energies->SetGuidance("A list of values is expected, with the "
                            "last parameter corresponding to the unit.");
  cmd_energies->SetParameterName("energies", false);

  cmd_particle = new G4UIcmdWithAString("/alpaca/particle", this);
  cmd_particle->SetGuidance("Add primary particle.");
  cmd_particle->SetGuidance("Default: gamma");
  cmd_particle->SetParameterName("particlename", true);
  cmd_particle->SetDefaultValue("gamma");
}

void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand *command,
                                            G4String str) {
  if (command == cmd_cascade) {
    action->set_cascade(str);
  } else if (command == cmd_energies) {
    action->set_energies(str);
  } else if (command == cmd_particle) {
    action->set_particle(str);
  }
}
