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

// Properties of available HPGe detectors
#pragma once

#include "G4SystemOfUnits.hh"

#include "HPGe_Clover_Properties.hh"
#include "HPGe_Coaxial_Properties.hh"
	
namespace HPGe_Coaxial_Collection{
	
	const double inch = 25.4*mm;

	// 60% coaxial HPGe with ORTEC serial number 36-TN21033A
	// Property of TUNL
	HPGe_Coaxial_Properties HPGe_60_TUNL_21033{
		.detector_radius = 0.5*68.*mm, // ORTEC data sheet
		.detector_length = 80.5*mm, // ORTEC data sheet
		.detector_face_radius = 8.*mm, // (J) in ORTEC data sheet
		.hole_radius = 0.5*9.3*mm, // ORTEC data sheet
		.hole_depth = 73.1*mm, // ORTEC data sheet
		.hole_face_radius = 0.5*9.3*mm, // Estimated to be the same as hole radius
		.mount_cup_length = // Calculated from end cap length. Different from (F) in ORTEC data sheet, because it shows only the part which contains the detector crystal.
			10.5*inch // end_cap_length
			- 0.5*mm // end_cap_window_thickness
			- 4.*mm, // end_cap_to_crystal_gap_front
		.mount_cup_thickness = 0.76*mm, // (H) in ORTEC data sheet
		.mount_cup_base_thickness = 3.2*mm, // (C) in ORTEC data sheet
		.mount_cup_material = "G4_Al", // ORTEC data sheet
		.end_cap_to_crystal_gap_front = 4.*mm, // (B) in ORTEC data sheet
		.end_cap_to_crystal_gap_side = // Calculated from outer radius and other given dimensions
			0.5*85.*mm // end_cap_outer_radius
			-1.*mm // end_cap_thickness
			-0.76*mm // mount_cup_thickness
			-0.5*68.*mm, // detector_radius
		.end_cap_thickness = 1.*mm, // (I) in ORTEC data sheet
		.end_cap_length = 10.5*inch, // Measured at the detector
		.end_cap_outer_radius = 0.5*85.*mm, // Estimated
		.end_cap_window_thickness = 0.5*mm, // (D) in ORTEC data sheet
		.end_cap_material = "G4_Al", // ORTEC data sheet
		.end_cap_window_material = "G4_Be", // ORTEC data sheet
		.cold_finger_radius = 0.5*4.*mm, // Suggestion by B. Fallin, Duke University. Not in ORTEC data sheet
		.cold_finger_penetration_depth = 
			73.1*mm // hole_depth
			- 5.*mm, // Estimated
		.cold_finger_material = "G4_Cu", // Estimated
		.connection_length = 2.*inch, // Measured
		.connection_radius = 0.75*inch, // Estimated
		.dewar_offset = 1.*inch, // Measured
		.connection_material = "G4_Al", // Estimated
		.dewar_length = 9.*inch, // Measured
		.dewar_outer_radius = 3.*inch, // Measured
		.dewar_wall_thickness = 5.*mm, // Estimated
		.dewar_material = "G4_Al", // Estimated
	};


    // 100% (103.5% according to data sheet) coaxial HPGe 
    // with ORTEC serial number 42-TP41203A
    // Property of R.V.F. Janssens group, Argonne National Lab
	HPGe_Coaxial_Properties HPGe_ANL_41203{
		.detector_radius = 0.5*89.9*mm, // Data sheet
		.detector_length = 98.3*mm, // Data sheet
		.detector_face_radius = 2.*mm, // Estimated
		.hole_radius = 0.5*10.*mm, // Estimated from 60% ORTEC models
		.hole_depth = 0.8*98.3*mm, // 0.8*detector_length (Estimated)
		.hole_face_radius = 0.5*10.*mm, // hole_radius (Estimated)
		.mount_cup_length = 
			8.*inch // end_cap_length
			- 3.5*mm // end_cap_window_thickness
			- 6.5*mm, // end_cap_to_crystal_gap_front
		.mount_cup_thickness = 0.8*mm, // Estimated
		.mount_cup_base_thickness = 0.8*mm, // mount_cup_thickness
		.mount_cup_material = "G4_Al", // Estimated
		.end_cap_to_crystal_gap_front = 6.5*mm, // Data sheet
		.end_cap_to_crystal_gap_side = 
            0.5*4.25*inch // end_cap_outer_radius
		    - 3.5*mm // end_cap_thickness
			- 0.8*mm // mount_cup_thickness
		    - 0.5*89.9*mm, // detector_radius
		.end_cap_thickness = 3.5*mm, // 'Absorbing layer' in data sheet
		.end_cap_length = 8.*inch, // Data sheet
		.end_cap_outer_radius = 0.5*4.25*inch, // Data sheet
		.end_cap_window_thickness = 3.5*mm, // 'Absorbing layer' in data sheet
		.end_cap_material = "G4_Al", // Data sheet
		.end_cap_window_material = "G4_Al", // Data sheet
		.cold_finger_radius = 0.5*4.*mm, // Estimated from 60% ORTEC models
		.cold_finger_penetration_depth =
            0.8*98.3*mm // hole_depth
            - 5.*mm, // Estimated
		.cold_finger_material = "G4_Cu", // Estimated
		.connection_length = 4.5*inch, // Data sheet
		.connection_radius = 0.5*4.5*inch, // Data sheet
		.dewar_offset = 0.*inch,
		.connection_material = "G4_Al", // Estimated
		.dewar_length = 16.5*inch, // Data sheet
		.dewar_outer_radius = 0.5*9.*inch, // Data sheet
		.dewar_wall_thickness = 5.*mm, // Estimated
		.dewar_material = "G4_Al", // Estimated
    };

    // 100% coaxial HPGe 
    // with ORTEC serial number 43-TP31670A
    // Property of R.V.F. Janssens group, Argonne National Lab
    HPGe_Coaxial_Properties HPGe_ANL_31670{
		.detector_radius = 0.5*88.6*mm, // Data sheet
		.detector_length = 84.1*mm, // Data sheet
		.detector_face_radius = 2.*mm, // Estimated
		.hole_radius = 0.5*10.*mm, // Estimated from 60% ORTEC models
		.hole_depth = 0.8*98.3*mm, // 0.8*detector_length (Estimated)
		.hole_face_radius = 0.5*10.*mm, // hole_radius (Estimated)
		.mount_cup_length = 
			8.*inch // end_cap_length
			- 3.5*mm // end_cap_window_thickness
			- 6.5*mm, // end_cap_to_crystal_gap_front
		.mount_cup_thickness = 0.8*mm, // Estimated
		.mount_cup_base_thickness = 0.8*mm, // mount_cup_thickness
		.mount_cup_material = "G4_Al", // Estimated
		.end_cap_to_crystal_gap_front = 6.5*mm, // Data sheet
		.end_cap_to_crystal_gap_side = 
            0.5*4.25*inch // end_cap_outer_radius
		    - 3.5*mm // end_cap_thickness
			- 0.8*mm // mount_cup_thickness
		    - 0.5*89.9*mm, // detector_radius
		.end_cap_thickness = 3.5*mm, // 'Absorbing layer' in data sheet
		.end_cap_length = 8.*inch, // Data sheet
		.end_cap_outer_radius = 0.5*4.25*inch, // Data sheet
		.end_cap_window_thickness = 3.5*mm, // 'Absorbing layer' in data sheet
		.end_cap_material = "G4_Al", // Data sheet
		.end_cap_window_material = "G4_Al", // Data sheet
		.cold_finger_radius = 0.5*4.*mm, // Estimated from 60% ORTEC models
		.cold_finger_penetration_depth =
            0.8*98.3*mm // hole_depth
            - 5.*mm, // Estimated
		.cold_finger_material = "G4_Cu", // Estimated
		.connection_length = 4.5*inch, // Data sheet
		.connection_radius = 0.5*4.5*inch, // Data sheet
		.dewar_offset = 0.*inch,
		.connection_material = "G4_Al", // Estimated
		.dewar_length = 16.5*inch, // Data sheet
		.dewar_outer_radius = 0.5*9.*inch, // Data sheet
		.dewar_wall_thickness = 5.*mm, // Estimated
		.dewar_material = "G4_Al", // Estimated
    };

}

namespace HPGe_Clover_Collection{

    HPGe_Clover_Properties HPGe_Clover_Yale{
		.crystal_radius = 0.5*50.*mm,
		.crystal_length = 80.*mm,
		.crystal_face_radius = 2.*mm,
		.crystal_gap = 2.*mm,
		.end_cap_to_crystal_gap_front = 10.*mm,
		.vacuum_length = 125.*mm,
		.end_cap_front_side_length = 101.*mm,
		.end_cap_front_rounding_radius = 15.5*mm,
		.end_cap_front_length = 260.*mm,
		.end_cap_front_thickness = 1.*mm,
		.end_cap_window_thickness = 0.5*mm,
		.end_cap_back_side_length = 130.*mm,
		.end_cap_back_rounding_radius = 30.*mm,
		.end_cap_back_length = 220.*mm,
		.end_cap_back_thickness = 0.5*29.*mm,
		.end_cap_material = "G4_Al",
		.connection_length = 50.*mm,
		.connection_radius = 0.5*70.*mm, // Estimated
		.connection_material = "G4_Al",
		.dewar_length = 268.*mm,
		.dewar_outer_radius = 111.*mm,
		.dewar_wall_thickness = 5.*mm,
		.dewar_material = "G4_Al",
	};	
}
