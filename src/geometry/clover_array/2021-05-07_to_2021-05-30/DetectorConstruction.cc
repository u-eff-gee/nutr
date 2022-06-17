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
#include "Filters.hh"
#include "HPGe_Clover.hh"
#include "HPGe_Coaxial.hh"
#include "HPGe_Collection.hh"
#include "LaBr3Ce_3x3.hh"
#include "LeadShieldingUTR_2021-05-07_to_2021-05-31.hh"
#include "Mechanical.hh"
#include "ZeroDegree.hh"

vector<Detector *> detectors = {
    new LaBr3Ce_3x3(
        "labr_L1", 90. * deg, 0. * deg, 25.4 * mm + 52.0 * mm,
        {{{"G4_Cu", cu_labr_thick}, {"G4_Pb", 2. * pb_labr_thick}}, true}),
    new LaBr3Ce_3x3(
        "labr_L3", 90. * deg, 90. * deg, 25.4 * mm + 45.0 * mm,
        {{{"G4_Cu", cu_labr_thick}, {"G4_Pb", 2. * pb_labr_thick}}, true}),
    new LaBr3Ce_3x3(
        "labr_L5", 90. * deg, 180. * deg, 25.4 * mm + 38.0 * mm,
        {{{"G4_Cu", cu_labr_thick}, {"G4_Pb", 2. * pb_labr_thick}}, true}),
    new LaBr3Ce_3x3(
        "labr_L7", 90. * deg, 270. * deg, 25.4 * mm + 40.0 * mm,
        {{{"G4_Cu", 2. * cu_labr_thick}, {"G4_Pb", 2. * pb_labr_thick}}, true}),

    new HPGe_Clover(
        "clover_B1", HPGe_Clover_Collection::HPGe_Clover_Yale, 135. * deg,
        0. * deg, 8.00 * inch,
        {{{"G4_Cu", cu_clover_thick}, {"G4_Pb", 2. * pb_clover_thin}}, true},
        {}, 0.5 * pi),
    new HPGe_Clover(
        "clover_B2", HPGe_Clover_Collection::HPGe_Clover_Yale, 125.26 * deg,
        45. * deg, 8.00 * inch,
        {{{"G4_Cu", cu_clover_thick}, {"G4_Pb", pb_clover_thick}}, true}, {},
        0.),
    new HPGe_Clover(
        "clover_B4", HPGe_Clover_Collection::HPGe_Clover_Yale, 125.26 * deg,
        135. * deg, 8.00 * inch,
        {{{"G4_Cu", cu_clover_thick}, {"G4_Pb", pb_clover_thick}}, true}, {},
        0.),
    new HPGe_Clover(
        "clover_B5", HPGe_Clover_Collection::HPGe_Clover_Yale, 135. * deg,
        180. * deg, 8.00 * inch,
        {{{"G4_Cu", cu_clover_thick}, {"G4_Pb", pb_clover_thick}}, true}, {},
        -0.5 * pi),
    new HPGe_Clover(
        "clover_B6", HPGe_Clover_Collection::HPGe_Clover_Yale, 125.26 * deg,
        225. * deg, 8.00 * inch,
        {{{"G4_Cu", cu_clover_thick}, {"G4_Pb", 2. * pb_clover_thin}}, true},
        {}, 0.5 * pi),
    new HPGe_Clover(
        "clover_B6", HPGe_Clover_Collection::HPGe_Clover_Yale, 125.26 * deg,
        315. * deg, 8.00 * inch,
        {{{"G4_Cu", cu_clover_thick}, {"G4_Pb", pb_clover_thick}}, true}, {},
        1.0 * pi),

    new CeBr3_2x2("cebr_B", 90. * deg, 27.5 * deg, 8.00 * inch,
                  {{{"G4_Cu", cu_cebr_thin}, {"G4_Pb", pb_cebr_thin}}, true}),
    new CeBr3_2x2("cebr_C", 90. * deg, 45. * deg, 8.00 * inch,
                  {{{"G4_Cu", cu_cebr_thin}, {"G4_Pb", pb_cebr_thin}}, true}),
    new CeBr3_2x2("cebr_D", 90. * deg, 62.5 * deg, 8.00 * inch,
                  {{{"G4_Cu", cu_cebr_thin}, {"G4_Pb", pb_cebr_thin}}, true}),
    new CeBr3_2x2("cebr_F", 90. * deg, 117.5 * deg, 8.00 * inch,
                  {{{"G4_Cu", cu_cebr_thin}, {"G4_Pb", pb_cebr_thin}}, true}),
    new CeBr3_2x2("cebr_G", 90. * deg, 135. * deg, 8.00 * inch,
                  {{{"G4_Cu", cu_cebr_thin}, {"G4_Pb", pb_cebr_thin}}, true}),
    new CeBr3_2x2("cebr_H", 90. * deg, 152.5 * deg, 8.00 * inch,
                  {{{"G4_Cu", cu_cebr_thin}, {"G4_Pb", pb_cebr_thin}}, true}),
    new CeBr3_2x2("cebr_K", 90. * deg, 225. * deg, 8.00 * inch,
                  {{{"G4_Cu", cu_cebr_thin}, {"G4_Pb", pb_cebr_thin}}, true}),
    new CeBr3_2x2("cebr_O", 90. * deg, 315. * deg, 8.00 * inch,
                  {{{"G4_Cu", cu_cebr_thin}, {"G4_Pb", pb_cebr_thin}}, true}),
    new CeBr3_2x2("cebr_BD", 135. * deg, 90. * deg, 8.00 * inch,
                  {{{"G4_Cu", cu_cebr_thin}, {"G4_Pb", pb_cebr_thin}}, true}),
    new CeBr3_2x2("cebr_BK", 135. * deg, 270. * deg, 8.00 * inch,
                  {{{"G4_Cu", cu_cebr_thin}, {"G4_Pb", pb_cebr_thin}}, true}),

    new HPGe_Coaxial("zero_degree",
                     HPGe_Coaxial_Collection::HPGe_120_TUNL_40383,
                     HPGe_Coaxial_Dewar_Properties(), 0. * deg, 0. * deg,
                     ZeroDegree::zero_degree_to_target),
};

G4VPhysicalVolume *DetectorConstruction::Construct() {

  G4NistManager *nist_manager = G4NistManager::Instance();

  world_solid = new G4Box("world_solid", 2. * m, 2. * m, 4.0 * m);
  world_logical = new G4LogicalVolume(
      world_solid, nist_manager->FindOrBuildMaterial("G4_AIR"),
      "world_logical");
  world_logical->SetVisAttributes(G4VisAttributes::GetInvisible());
  world_phys = new G4PVPlacement(new G4RotationMatrix(), G4ThreeVector(),
                                 world_logical, "world", nullptr, false, 0);

  CollimatorRoom(world_logical).Construct({});
  BeamPipe(world_logical).Construct({});
  LeadShieldingUTR(world_logical).Construct({});
  Mechanical(world_logical).Construct({});

  // Offsets of LaBr detectors in y direction need to be treated separately.
  detectors[0]->Construct(world_logical, {});
  RegisterSensitiveLogicalVolumes(
      detectors[0]->get_sensitive_logical_volumes());
  detectors[1]->Construct(world_logical, {-4. * mm, 0., 0.});
  RegisterSensitiveLogicalVolumes(
      detectors[1]->get_sensitive_logical_volumes());
  detectors[2]->Construct(world_logical, {0., -4. * mm, 0.});
  RegisterSensitiveLogicalVolumes(
      detectors[2]->get_sensitive_logical_volumes());
  detectors[3]->Construct(world_logical, {});
  RegisterSensitiveLogicalVolumes(
      detectors[3]->get_sensitive_logical_volumes());
  for (size_t n_detector = 4; n_detector < detectors.size() - 1; ++n_detector) {
    detectors[n_detector]->Construct(world_logical, {});
    RegisterSensitiveLogicalVolumes(
        detectors[n_detector]->get_sensitive_logical_volumes());
  }
  detectors[detectors.size() - 1]->Construct(
      world_logical, G4ThreeVector(0., ZeroDegree::offset_y, 0.));
  RegisterSensitiveLogicalVolumes(
      detectors[detectors.size() - 1]->get_sensitive_logical_volumes());

  return world_phys;
}
