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

#include <memory>

using std::make_unique;

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

#include "DetectorConstruction.hh"

#include "HPGe_Clover.hh"
#include "HPGe_Collection.hh"

G4VPhysicalVolume* DetectorConstruction::Construct()
{

	G4NistManager *nist_manager = G4NistManager::Instance();

    const double distance = 7.*25.4*mm;

	world_solid = make_unique<G4Box>("world_solid", 2.*m, 2.*m, 2.*m);
	world_logical = make_unique<G4LogicalVolume>(world_solid.get(), nist_manager->FindOrBuildMaterial("G4_AIR"), "world_logical");
	world_logical->SetVisAttributes(G4VisAttributes::GetInvisible());
	world_phys = make_unique<G4PVPlacement>(new G4RotationMatrix(), G4ThreeVector(), world_logical.get(), "world", nullptr, false, 0);

	HPGe_Clover clover(world_logical.get(), "clover", HPGe_Clover_Collection::HPGe_Clover_Yale);
    clover.useDewar();
	clover.Construct(G4ThreeVector(), 0.5*pi, 0., distance);
    RegisterSensitiveLogicalVolumes(clover.get_sensitive_logical_volumes());

	HPGe_Clover clover2(world_logical.get(), "clover2", HPGe_Clover_Collection::HPGe_Clover_Yale);
    clover2.useDewar();
	clover2.Construct(G4ThreeVector(), 0.5*pi, 0.5*pi, distance);

	HPGe_Clover clover3(world_logical.get(), "clover3", HPGe_Clover_Collection::HPGe_Clover_Yale);
    clover3.useDewar();
	clover3.Construct(G4ThreeVector(), 116./180.*pi, 0.25*pi, distance, 0.2*pi);

	return world_phys.get();
}
