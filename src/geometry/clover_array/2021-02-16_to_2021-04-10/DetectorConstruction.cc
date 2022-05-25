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
#include "ComptonMonitor_2021-02-16_to_2021-04-18.hh"
#include "HPGe_Clover.hh"
#include "HPGe_Coaxial.hh"
#include "HPGe_Collection.hh"
#include "LaBr3Ce_3x3.hh"
#include "LeadShieldingUTR.hh"
#include "Mechanical.hh"
#include "ZeroDegree.hh"

constexpr double inch = 25.4 * mm;

constexpr double cu_thin = 0.040 * inch;
constexpr double pb_thin = 0.046 * inch;
constexpr double pb_thinner = 0.038 * inch;
constexpr double pb_thick = 0.10 * inch;

vector<Detector *> detectors = {
    new HPGe_Clover("clover_1", HPGe_Clover_Collection::HPGe_Clover_Yale,
                    90. * deg, 0. * deg, 8.00 * inch,
                    {{{"G4_Cu", cu_thin}, {"G4_Pb", pb_thin}}, true}, {},
                    1.5 * pi),
    new HPGe_Clover("clover_3", HPGe_Clover_Collection::HPGe_Clover_Yale,
                    90. * deg, 90. * deg, 8.00 * inch,
                    {{{"G4_Cu", cu_thin}, {"G4_Pb", pb_thin}}, true}, {},
                    1.5 * pi),
    new HPGe_Clover("clover_5", HPGe_Clover_Collection::HPGe_Clover_Yale,
                    90. * deg, 180. * deg, 8.00 * inch,
                    {{{"G4_Cu", cu_thin}, {"G4_Pb", pb_thin}}, true}, {},
                    0.5 * pi),
    new HPGe_Clover("clover_7", HPGe_Clover_Collection::HPGe_Clover_Yale,
                    90. * deg, 270. * deg, 8.00 * inch,
                    {{{"G4_Cu", cu_thin}, {"G4_Pb", pb_thin}}, true}, {}, 0.),

    new HPGe_Clover("clover_B1", HPGe_Clover_Collection::HPGe_Clover_Yale,
                    135. * deg, 0. * deg, 8.00 * inch,
                    {{{"G4_Cu", cu_thin}, {"G4_Pb", pb_thin}}, true}, {},
                    0.5 * pi),
    new HPGe_Clover("clover_B4", HPGe_Clover_Collection::HPGe_Clover_Yale,
                    125.26 * deg, 135. * deg, 8.00 * inch,
                    {{{"G4_Cu", cu_thin}, {"G4_Pb", pb_thin}}, true}, {},
                    0.5 * pi),
    new HPGe_Clover("clover_B5", HPGe_Clover_Collection::HPGe_Clover_Yale,
                    135. * deg, 180. * deg, 8.00 * inch,
                    {{{"G4_Cu", cu_thin}, {"G4_Pb", pb_thin}}, true}, {},
                    1.5 * pi),
    new HPGe_Clover("clover_B6", HPGe_Clover_Collection::HPGe_Clover_Yale,
                    125.26 * deg, 225. * deg, 8.00 * inch,
                    {{{"G4_Cu", cu_thin}, {"G4_Pb", pb_thin}}, true}, {},
                    1.5 * pi),

    new CeBr3_2x2("cebr_B", 90. * deg, 27.5 * deg, 8.00 * inch,
                  {{{"G4_Cu", cu_thin}, {"G4_Pb", pb_thin}}, true}),
    new CeBr3_2x2("cebr_C", 90. * deg, 62.5 * deg, 8.00 * inch,
                  {{{"G4_Cu", cu_thin}, {"G4_Pb", pb_thin}}, true}),
    new CeBr3_2x2("cebr_E", 90. * deg, 117.5 * deg, 8.00 * inch,
                  {{{"G4_Cu", cu_thin}, {"G4_Pb", pb_thin}}, true}),
    new CeBr3_2x2("cebr_F", 90. * deg, 152.5 * deg, 8.00 * inch,
                  {{{"G4_Cu", cu_thin}, {"G4_Pb", pb_thin}}, true}),
    new CeBr3_2x2("cebr_I", 90. * deg, 225.0 * deg, 8.00 * inch,
                  {{{"G4_Cu", cu_thin}, {"G4_Pb", pb_thin}}, true}),
    new CeBr3_2x2("cebr_M", 90. * deg, 315.0 * deg, 8.00 * inch,
                  {{{"G4_Cu", cu_thin}, {"G4_Pb", pb_thin}}, true}),

    new CeBr3_2x2("cebr_BB", 135. * deg, 3. / 14. * 180. * deg, 8.00 * inch,
                  {{{"G4_Cu", cu_thin}, {"G4_Pb", pb_thin}}, true}),
    new CeBr3_2x2("cebr_BC", 135. * deg, 5. / 14. * 180. * deg, 8.00 * inch,
                  {{{"G4_Cu", cu_thin}, {"G4_Pb", pb_thin}}, true}),
    new CeBr3_2x2("cebr_BD", 135. * deg, 7. / 14. * 180. * deg, 8.00 * inch,
                  {{{"G4_Cu", cu_thin}, {"G4_Pb", pb_thin}}, true}),
    new CeBr3_2x2("cebr_BK", 135. * deg, 21. / 14. * 180. * deg, 8.00 * inch,
                  {{{"G4_Cu", cu_thin}, {"G4_Pb", pb_thin}}, true}),
    new CeBr3_2x2("cebr_BL", 135. * deg, 23. / 14. * 180. * deg, 8.00 * inch,
                  {{{"G4_Cu", cu_thin}, {"G4_Pb", pb_thin}}, true}),
    new CeBr3_2x2("cebr_BM", 135. * deg, 25. / 14. * 180. * deg, 8.00 * inch,
                  {{{"G4_Cu", cu_thin}, {"G4_Pb", pb_thin}}, true}),

    new HPGe_Coaxial("zero_degree",
                     HPGe_Coaxial_Collection::HPGe_120_TUNL_40383, 0. * deg,
                     0. * deg, ZeroDegree::zero_degree_to_target),
    new LaBr3Ce_3x3("labr_Z", ComptonMonitor::detector_angle, 0. * deg,
                    ComptonMonitor::scattering_target_to_detector),
};

G4VPhysicalVolume *DetectorConstruction::Construct() {

  G4NistManager *nist_manager = G4NistManager::Instance();

  world_solid = new G4Box("world_solid", 2. * m, 2. * m, 3.5 * m);
  world_logical = new G4LogicalVolume(
      world_solid, nist_manager->FindOrBuildMaterial("G4_AIR"),
      "world_logical");
  world_logical->SetVisAttributes(G4VisAttributes::GetInvisible());
  world_phys = new G4PVPlacement(new G4RotationMatrix(), G4ThreeVector(),
                                 world_logical, "world", nullptr, false, 0);

  CollimatorRoom(world_logical).Construct({});
  ComptonMonitor(world_logical).Construct({});
  BeamPipe(world_logical).Construct({});
  LeadShieldingUTR(world_logical).Construct({});
  Mechanical(world_logical).Construct({});

  for (size_t n_detector = 0; n_detector < detectors.size() - 2; ++n_detector) {
    detectors[n_detector]->Construct(world_logical, {});
    RegisterSensitiveLogicalVolumes(
        detectors[n_detector]->get_sensitive_logical_volumes());
  }
  detectors[detectors.size() - 2]->Construct(
      world_logical, G4ThreeVector(0., ZeroDegree::offset_y, 0.));
  detectors[detectors.size() - 1]->Construct(
      world_logical,
      G4ThreeVector(0., 0., ComptonMonitor::scattering_target_to_target));

  return world_phys;
}
