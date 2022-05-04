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

#include <string>

using std::string;

#include <boost/program_options.hpp>

namespace po = boost::program_options;

#include "G4RunManagerFactory.hh"

#include "G4UImanager.hh"

#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"

#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "Physics.hh"

int main(int argc, char **argv) {
  po::options_description desc("nutr: new utr - program options");
  desc.add_options()("help", "Show help message.")(
      "macro", po::value<string>(),
      "Name of a Geant4 macro file to be executed. If no macro file is given, "
      "nutr tries to launch the interactive user interface and a visualization "
      "of the geometry.")(
      "output", po::value<string>()->default_value(""),
      "Output file name. Please note that, in Geant4, the suffix of the output "
      "file determines the output format. If no output file name is specified, "
      "a time stamp is used. Default: \"\", i.e. use time stamp.")(
      "seed", po::value<long>()->default_value(1),
      "Set random-number seed. Default: 1.");
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    G4cout << desc << G4endl;
    return 1;
  }

  G4UIExecutive *ui = nullptr;
  if (!vm.count("macro")) {
    ui = new G4UIExecutive(argc, argv);
  }

  G4Random::setTheSeed(vm["seed"].as<long>());

  auto *runManager =
      G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

  runManager->SetUserInitialization(new DetectorConstruction());

  G4VModularPhysicsList *physicsList = new Physics();
  physicsList->SetCuts();
  runManager->SetUserInitialization(physicsList);

  runManager->SetUserInitialization(new ActionInitialization(
      vm["output"].as<string>(), vm["seed"].as<long>()));

  G4VisManager *visManager = new G4VisExecutive();
  visManager->Initialize();
  G4UImanager *UImanager = G4UImanager::GetUIpointer();

  if (!ui) {
    G4String command = "/control/execute ";
    G4String fileName = vm["macro"].as<string>();
    UImanager->ApplyCommand(command + fileName);
  } else {
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }
}
