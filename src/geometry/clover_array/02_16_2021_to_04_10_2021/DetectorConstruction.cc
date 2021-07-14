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

#include <array>

using std::array;

#include <memory>

using std::make_unique;

#include <string>

using std::string;

#include <vector>

using std::vector;

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

#include "DetectorConstruction.hh"

#include "BeamPipe.hh"
#include "CeBr3_15x15.hh"
#include "CollimatorRoom.hh"
#include "ComptonMonitor_02_16_2021_to_04_18_2021.hh"
#include "HPGe_Clover.hh"
#include "HPGe_Collection.hh"
#include "LaBr3Ce_3x3.hh"
#include "LeadShieldingUTR.hh"

const double distance = 8.*25.4*mm;

struct DetectorPosition{
    const string id;
    const double theta;
    const double phi;
    const double distance;
    const double intrinsic_rotation_angle;
};

array<DetectorPosition, 8> clover_position{
    DetectorPosition{"clover_1",         0.5 *pi, 0.     , distance, 0.5*pi},
    // DetectorPosition{"clover_2",         0.5 *pi, 0.25*pi, distance, 0.},
    DetectorPosition{"clover_3",         0.5 *pi, 0.5 *pi, distance, 1.5*pi},
    // DetectorPosition{"clover_4",         0.5 *pi, 0.75*pi, distance, 0.},
    DetectorPosition{"clover_5",         0.5 *pi,      pi, distance, 0.5*pi},
    // DetectorPosition{"clover_6",         0.5 *pi, 1.25*pi, distance, 0.},
    DetectorPosition{"clover_7",         0.5 *pi, 1.5 *pi, distance, 0.5*pi},
    // DetectorPosition{"clover_8",         0.5 *pi, 1.75*pi, distance, 0.},
    DetectorPosition{"clover_B1",        0.75*pi, 0.     , distance, 0.5*pi},
    // DetectorPosition{"clover_B2", 125.26/180.*pi, 0.25*pi, distance, 0.5*pi},
    DetectorPosition{"clover_B4", 125.26/180.*pi, 0.75*pi, distance, 0.    },
    DetectorPosition{"clover_B5",        0.75*pi,      pi, distance, 1.5*pi},
    DetectorPosition{"clover_B6", 125.26/180.*pi, 1.25*pi, distance, 0.5*pi},
    // DetectorPosition{"clover_B8", 125.26/180.*pi, 1.75*pi, distance, 0.},
};

array<DetectorPosition, 12> cebr_position{
    DetectorPosition{"cebr_B",           0.5 *pi, 27.5  /180.*pi, distance, 0.    },
    // DetectorPosition{"cebr_C",           0.5 *pi, 0.25       *pi, distance, 0.    },
    DetectorPosition{"cebr_D",           0.5 *pi, 62.5  /180.*pi, distance, 0.    },
    DetectorPosition{"cebr_F",           0.5 *pi, 117.5 /180.*pi, distance, 0.    },
    // DetectorPosition{"cebr_G",           0.5 *pi, 0.75       *pi, distance, 0.    },
    DetectorPosition{"cebr_H",           0.5 *pi, 152.5 /180.*pi, distance, 0.    },
    DetectorPosition{"cebr_K",           0.5 *pi,        1.25*pi, distance, 0.    },
    DetectorPosition{"cebr_O",           0.5 *pi,        1.75*pi, distance, 0.    },
    // DetectorPosition{"cebr_BA",          0.75*pi, (0.5-3./7.)*pi, distance, 0.    },
    DetectorPosition{"cebr_BB",          0.75*pi, (0.5-2./7.)*pi, distance, 0.    },
    DetectorPosition{"cebr_BC",          0.75*pi, (0.5-1./7.)*pi, distance, 0.    },
    DetectorPosition{"cebr_BD",          0.75*pi, 0.5        *pi, distance, 0.    },
    // DetectorPosition{"cebr_BE",          0.75*pi, (0.5+1./7.)*pi, distance, 0.    },
    // DetectorPosition{"cebr_BF",          0.75*pi, (0.5+2./7.)*pi, distance, 0.    },
    // DetectorPosition{"cebr_BG",          0.75*pi, (0.5+3./7.)*pi, distance, 0.    },
    // DetectorPosition{"cebr_BH",          0.75*pi, (0.5+4./7.)*pi, distance, 0.    },
    // DetectorPosition{"cebr_BI",          0.75*pi, (0.5+5./7.)*pi, distance, 0.    },
    // DetectorPosition{"cebr_BJ",          0.75*pi, (0.5+6./7.)*pi, distance, 0.    },
    DetectorPosition{"cebr_BK",          0.75*pi, 1.5        *pi, distance, 0.    },
    DetectorPosition{"cebr_BL",          0.75*pi, (1.5+1./7.)*pi, distance, 0.    },
    DetectorPosition{"cebr_BM",          0.75*pi, (1.5+2./7.)*pi, distance, 0.    },
    // DetectorPosition{"cebr_BN",          0.75*pi, (1.5+3./7.)*pi, distance, 0.    },
};

G4VPhysicalVolume* DetectorConstruction::Construct()
{

	G4NistManager *nist_manager = G4NistManager::Instance();

	world_solid = make_unique<G4Box>("world_solid", 2.*m, 2.*m, 3.5*m);
	world_logical = make_unique<G4LogicalVolume>(world_solid.get(), nist_manager->FindOrBuildMaterial("G4_AIR"), "world_logical");
	world_logical->SetVisAttributes(G4VisAttributes::GetInvisible());
	world_phys = make_unique<G4PVPlacement>(new G4RotationMatrix(), G4ThreeVector(), world_logical.get(), "world", nullptr, false, 0);

    CollimatorRoom collimator_room(world_logical.get());
    collimator_room.Construct({});

    BeamPipe beam_pipe(world_logical.get());
    beam_pipe.Construct({});

    LeadShieldingUTR lead_shielding_UTR(world_logical.get());
    lead_shielding_UTR.Construct({});

    vector<HPGe_Clover> clovers;
    for(auto det_pos: clover_position){
        clovers.push_back(HPGe_Clover(world_logical.get(), det_pos.id, HPGe_Clover_Collection::HPGe_Clover_Yale));
        clovers[clovers.size()-1].Construct(G4ThreeVector(), det_pos.theta, det_pos.phi, det_pos.distance, det_pos.intrinsic_rotation_angle);
        RegisterSensitiveLogicalVolumes(clovers[clovers.size()-1].get_sensitive_logical_volumes());
    }

    vector<CeBr3_15x15> cebrs;
    for(auto det_pos: cebr_position){
        cebrs.push_back(CeBr3_15x15(world_logical.get(), det_pos.id));
        cebrs[cebrs.size()-1].Construct(G4ThreeVector(), det_pos.theta, det_pos.phi, det_pos.distance, det_pos.intrinsic_rotation_angle);
        RegisterSensitiveLogicalVolumes(cebrs[cebrs.size()-1].get_sensitive_logical_volumes());
    }

    ComptonMonitor compton_monitor(world_logical.get());
    compton_monitor.Construct({});
    LaBr3Ce_3x3 compton_monitor_detector(world_logical.get(), "Z");
    RegisterSensitiveLogicalVolumes(compton_monitor_detector.get_sensitive_logical_volumes());
    compton_monitor_detector.Construct({0., 0., ComptonMonitor::scattering_target_to_target}, ComptonMonitor::detector_angle, 0., ComptonMonitor::scattering_target_to_detector);

	return world_phys.get();
}
