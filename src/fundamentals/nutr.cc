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

    Copyright (C) 2020-2022 Udo Friman-Gayer
*/

#include <string>

using std::string;

#include "G4RunManagerFactory.hh"

#include "G4UImanager.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"


#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "Physics.hh"

int main(int argc, char** argv)
{
    G4UIExecutive* ui = nullptr;
    if ( argc == 1 ) {
        ui = new G4UIExecutive(argc, argv);
    }
    const long seed = argc == 3 ? atoi(argv[2]) : 1;
    G4Random::setTheSeed(seed);

    auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

    runManager->SetUserInitialization(new DetectorConstruction());

    G4VModularPhysicsList* physicsList = new Physics();
    physicsList->SetCuts();
    runManager->SetUserInitialization(physicsList);

    string output_file_name = "";
    if(argc == 4){
        output_file_name = argv[3];
    }
    runManager->SetUserInitialization(new ActionInitialization(output_file_name, seed));

    G4VisManager* visManager = new G4VisExecutive();
    visManager->Initialize();
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    if ( ! ui ) { 
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);
    }
    else { 
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        ui->SessionStart();
        delete ui;
    }
}
