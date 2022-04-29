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
        Copyright (C) 2021 Oliver Papst
*/

#include <memory>
#include <vector>

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

#include "DetectorConstruction.hh"

#include "BeamPipe.hh"
#include "CeBr3_2x2.hh"
#include "CollimatorRoom.hh"
#include "ComptonMonitor_2021-09-09_to_2021-10-10.hh"
#include "HPGe_Clover.hh"
#include "HPGe_Coaxial.hh"
#include "HPGe_Collection.hh"
#include "LaBr3Ce_3x3.hh"
#include "LeadShieldingUTR.hh"
#include "Target96Mo.hh"
#include "CoaxB4Config.hh"

using std::make_unique;

constexpr double inch = 25.4 * mm;

constexpr double cu_thin = 0.040 * inch;
constexpr double pb_thin = 0.046 * inch;
constexpr double pb_thinner = 0.038 * inch;
constexpr double pb_thick = 0.10 * inch;

std::vector<DetectorPosition> clover_position {
    {"clover_3", 90. * deg, 90. * deg, 6.68 * inch, 1.5 * pi, pb_thin, cu_thin},
    {"clover_5", 90. * deg, 180. * deg, 5.88 * inch, 0.5 * pi, pb_thick, cu_thin},
    {"clover_8", 90. * deg, 315. * deg, 6.38 * inch, 0., pb_thin, cu_thin},

    {"clover_B1", 135. * deg, 0., 7.75 * inch, 0.5 * pi, pb_thin, cu_thin},
    {"clover_B2", 125.26 * deg, 45. * deg, 7.75 * inch, 0.5 * pi, pb_thin, cu_thin},
    {"clover_B5", 135. * deg, 180. * deg, 8.00 * inch, 1.5 * pi, pb_thin, cu_thin},
};

auto coaxial_B4_position = DetectorPosition {"coaxial_B4", 125.26 * deg, 135. * deg, 9.25 * inch, 0., pb_thinner, cu_thin};

std::vector<DetectorPosition> cebr_position {
    {"cebr_B", 90. * deg, 27.5 * deg, 8.0 * inch, 0., 0., cu_thin},
    {"cebr_C", 90. * deg, 45.0 * deg, 8.0 * inch, 0., 0., cu_thin},
    {"cebr_D", 90. * deg, 62.5 * deg, 8.0 * inch, 0., 0., cu_thin},
    {"cebr_F", 90. * deg, 117.5 * deg, 8.0 * inch, 0., 0., cu_thin},
    {"cebr_G", 90. * deg, 135.0 * deg, 8.0 * inch, 0., 0., cu_thin},
    {"cebr_H", 90. * deg, 152.5 * deg, 8.0 * inch, 0., 0., cu_thin},
    {"cebr_K", 90. * deg, 225.0 * deg, 5.75 * inch, 0., 0., cu_thin},

    {"cebr_BD", 135. * deg, 90. * deg, 6.5 * inch, 0., 0., cu_thin},
    {"cebr_BK", 135. * deg, 270. * deg, 7.13 * inch, 0., 0., cu_thin},

    {"cebr_Z", 35. * deg, 0. * pi, 57. * inch, 0.},
};

std::vector<DetectorPosition> labr_position {
    {"labr_L1", 90. * deg, 0. * deg, 1.75 * inch, 0., pb_thick},
    {"labr_L7", 90. * deg, 270. * deg, 2.38 * inch, 0., pb_thick},

    {"labr_LBI", 135. * deg, 225. * deg, 4.75 * inch, 0., pb_thick},
    {"labr_LBM", 135. * deg, 315. * deg, 4.50 * inch, 0., pb_thick},
};

template <typename Detector, typename... Args>
void DetectorConstruction::construct_detector(DetectorPosition det_pos,
    Args... args)
{
    auto det = Detector(world_logical.get(), det_pos.id, args...);
    if(det_pos.filter_pb != 0.) {
        det.Add_Filter("G4_Pb", det_pos.filter_pb);
    }
    if(det_pos.filter_cu != 0.) {
        det.Add_Filter("G4_Cu", det_pos.filter_cu);
    }
    det.Construct({}, det_pos.theta, det_pos.phi, det_pos.distance,
        det_pos.intrinsic_rotation_angle);
    RegisterSensitiveLogicalVolumes(det.get_sensitive_logical_volumes());
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{

    G4NistManager* nist_manager = G4NistManager::Instance();

    world_solid = make_unique<G4Box>("world_solid", 2. * m, 2. * m, 3.5 * m);
    world_logical = make_unique<G4LogicalVolume>(
        world_solid.get(), nist_manager->FindOrBuildMaterial("G4_AIR"),
        "world_logical");
    world_logical->SetVisAttributes(G4VisAttributes::GetInvisible());
    world_phys = make_unique<G4PVPlacement>(new G4RotationMatrix(),
        G4ThreeVector(), world_logical.get(),
        "world", nullptr, false, 0);

    CollimatorRoom(world_logical.get()).Construct({});
    BeamPipe(world_logical.get()).Construct({});
    LeadShieldingUTR(world_logical.get()).Construct({});

    for(auto& det_pos: clover_position) {
        construct_detector<HPGe_Clover>(det_pos,
            HPGe_Clover_Collection::HPGe_Clover_Yale);
    }

    construct_detector<HPGe_Coaxial>(
        coaxial_B4_position, COAXIAL_B4, false, false, true);

    for(auto& det_pos: cebr_position) {
        construct_detector<CeBr3_2x2>(det_pos);
    }

    for(auto& det_pos: labr_position) {
        construct_detector<LaBr3Ce_3x3>(det_pos);
    }

    Target96Mo(world_logical.get()).Construct({});

    return world_phys.get();
}
