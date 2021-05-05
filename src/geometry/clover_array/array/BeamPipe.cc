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

#include "G4NistManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"

#include "BeamPipe.hh"

void BeamPipe::Construct(const G4ThreeVector global_coordinates){
    
    const double inch = 25.4*mm;

    const double beam_pipe_length = 2.*m; // Estimated
    const double beam_pipe_outer_radius = 1.*inch;
    const double beam_pipe_inner_radius = 0.875*inch;

    const double insert_pipe_lid_thickness = 2.*mm; // Estimated
    const double insert_pipe_lid_outer_radius = 1.08*inch; // Estimated
    const double insert_pipe_inner_diameter = 0.8*inch;
    const double insert_pipe_front_inner_diameter = 0.85*inch; // Estimated
    const double beam_pipe_downstream_lid_to_target_position = 23.75*inch;
    const double insert_pipe_front_length = 2.*inch; // Estimated
    const double insert_pipe_main_length = beam_pipe_downstream_lid_to_target_position - insert_pipe_lid_thickness - 0.5*insert_pipe_front_length;

    G4NistManager* nist = G4NistManager::Instance();

    // Main pipe

    G4Tubs* beam_pipe_solid = new G4Tubs("beam_pipe_solid", beam_pipe_inner_radius, beam_pipe_outer_radius, 0.5*beam_pipe_length, 0., twopi);
    G4LogicalVolume* beam_pipe_logical = new G4LogicalVolume(beam_pipe_solid, nist->FindOrBuildMaterial("G4_PLEXIGLASS"), "beam_pipe_logical");
    new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 0., 0.), beam_pipe_logical, "beam_pipe", world_logical, false, 0, false);
}