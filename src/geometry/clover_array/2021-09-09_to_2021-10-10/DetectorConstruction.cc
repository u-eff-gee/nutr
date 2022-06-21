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

#include <memory>
#include <vector>

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "DetectorConstruction.hh"

#include "BeamPipe.hh"
#include "CeBr3_2x2.hh"
#include "CoaxB4Config.hh"
#include "CollimatorRoom.hh"
#include "ComptonMonitor_2021-09-09_to_2021-10-10.hh"
#include "HPGe_Clover.hh"
#include "HPGe_Coaxial.hh"
#include "HPGe_Collection.hh"
#include "LaBr3Ce_3x3.hh"
#include "LeadShieldingUTR_2021-09-10_to_2021-10-10.hh"
#include "Mechanical.hh"
#include "Target96Mo.hh"
#include "ZeroDegreeMechanical.hh"

constexpr double inch = 25.4 * mm;

constexpr double cu_thin = 0.040 * inch;
constexpr double pb_thin = 0.046 * inch;
constexpr double pb_thinner = 0.038 * inch;
constexpr double pb_thick = 0.10 * inch;

vector<Detector *> detectors = {
    new HPGe_Clover("clover_3", HPGe_Clover_Collection::HPGe_Clover_Yale,
                    90. * deg, 90. * deg, 6.68 * inch,
                    {{{"G4_Cu", cu_thin}, {"G4_Pb", pb_thin}}, true}, {},
                    1.5 * pi),
    new HPGe_Clover("clover_5", HPGe_Clover_Collection::HPGe_Clover_Yale,
                    90. * deg, 180. * deg, 5.88 * inch,
                    {{{"G4_Cu", cu_thin}, {"G4_Pb", pb_thin}}, true}, {},
                    0.5 * pi),
    new HPGe_Clover("clover_8", HPGe_Clover_Collection::HPGe_Clover_Yale,
                    90. * deg, 315. * deg, 6.38 * inch,
                    {{{"G4_Cu", cu_thin}, {"G4_Pb", pb_thin}}, true}, {}, 0.),

    new HPGe_Clover("clover_B1", HPGe_Clover_Collection::HPGe_Clover_Yale,
                    135. * deg, 0. * deg, 7.75 * inch,
                    {{{"G4_Cu", cu_thin}, {"G4_Pb", pb_thin}}, true}, {},
                    0.5 * pi),
    new HPGe_Clover("clover_B2", HPGe_Clover_Collection::HPGe_Clover_Yale,
                    125.26 * deg, 45. * deg, 7.75 * inch,
                    {{{"G4_Cu", cu_thin}, {"G4_Pb", pb_thin}}, true}, {},
                    0.5 * pi),
    new HPGe_Coaxial(
        "coaxial_B4", COAXIAL_B4, COAXIAL_B4_DEWAR, 125.26 * deg, 135. * deg,
        9.25 * inch,
        {
            {{"G4_Cu",
              [](const string name) {
                return new G4Box(name, 50.5 * mm, 50.5 * mm, 0.5 * cu_thin);
              },
              cu_thin},
             {"G4_Pb",
              [](const string name) {
                return new G4Box(name, 50.5 * mm, 50.5 * mm, 0.5 * pb_thinner);
              },
              pb_thinner}},
        }),
    new HPGe_Clover("clover_B5", HPGe_Clover_Collection::HPGe_Clover_Yale,
                    135. * deg, 180. * deg, 8.00 * inch,
                    {{{"G4_Cu", cu_thin}, {"G4_Pb", pb_thin}}, true}, {},
                    1.5 * pi),

    new CeBr3_2x2("cebr_B", 90. * deg, 27.5 * deg, 8.0 * inch,
                  {{{"G4_Cu", cu_thin}}, true}),
    new CeBr3_2x2("cebr_C", 90. * deg, 45.0 * deg, 8.0 * inch,
                  {{{"G4_Cu", cu_thin}}, true}),
    new CeBr3_2x2("cebr_D", 90. * deg, 62.5 * deg, 8.0 * inch,
                  {{{"G4_Cu", cu_thin}}, true}),
    new CeBr3_2x2("cebr_F", 90. * deg, 117.5 * deg, 8.0 * inch,
                  {{{"G4_Cu", cu_thin}}, true}),
    new CeBr3_2x2("cebr_G", 90. * deg, 135.0 * deg, 8.0 * inch,
                  {{{"G4_Cu", cu_thin}}, true}),
    new CeBr3_2x2("cebr_H", 90. * deg, 152.5 * deg, 8.0 * inch,
                  {{{"G4_Cu", cu_thin}}, true}),
    new CeBr3_2x2("cebr_K", 90. * deg, 225.0 * deg, 5.75 * inch,
                  {{{"G4_Cu", cu_thin}}, true}),

    new CeBr3_2x2("cebr_BD", 135. * deg, 90.0 * deg, 6.5 * inch,
                  {{{"G4_Cu", cu_thin}}, true}),
    new CeBr3_2x2("cebr_BK", 135. * deg, 270.0 * deg, 7.13 * inch,
                  {{{"G4_Cu", cu_thin}}, true}),

    new CeBr3_2x2("cebr_Z", 35. * deg, 0. * deg, 57. * inch),

    new LaBr3Ce_3x3("labr_L1", 90. * deg, 0. * deg, 1.75 * inch,
                    {{{"G4_Pb", pb_thick}}}),
    new LaBr3Ce_3x3("labr_L7", 90. * deg, 270. * deg, 2.38 * inch,
                    {{{"G4_Pb", pb_thick}}}),
    new LaBr3Ce_3x3("labr_LBI", 135. * deg, 225. * deg, 4.75 * inch,
                    {{{"G4_Pb", pb_thick}}}),
    new LaBr3Ce_3x3("labr_LBM", 135. * deg, 315. * deg, 4.50 * inch,
                    {{{"G4_Pb", pb_thick}}}),

    new HPGe_Coaxial("zero_degree",
                     HPGe_Coaxial_Collection::HPGe_120_TUNL_40383,
                     HPGe_Coaxial_Dewar_Properties(), 0. * deg, 0. * deg,
                     ZeroDegreeMechanical::zero_degree_to_target),
};

G4VPhysicalVolume *DetectorConstruction::Construct() {

  ConstructBoxWorld(2. * m, 2. * m, 5.0 * m);

  CollimatorRoom(world_logical).Construct({});
  BeamPipe(world_logical).Construct({});
  LeadShieldingUTR(world_logical).Construct({});
  Mechanical(world_logical).Construct({});
  ZeroDegreeMechanical(world_logical).Construct({});

  for (size_t n_detector = 0; n_detector < detectors.size() - 1; ++n_detector) {
    detectors[n_detector]->Construct(world_logical, {});
    RegisterSensitiveLogicalVolumes(
        detectors[n_detector]->get_sensitive_logical_volumes());
  }
  detectors[detectors.size() - 1]->Construct(
      world_logical, G4ThreeVector(0., ZeroDegreeMechanical::offset_y, 0.));
  RegisterSensitiveLogicalVolumes(
      detectors[detectors.size() - 1]->get_sensitive_logical_volumes());

  Target96Mo(world_logical).Construct({});

  return world_phys;
}
