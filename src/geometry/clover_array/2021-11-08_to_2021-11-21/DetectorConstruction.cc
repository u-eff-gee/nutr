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
#include "CollimatorRoom.hh"
#include "Filters.hh"
#include "GammaVault.hh"
#include "HPGe_Clover.hh"
#include "HPGe_Coaxial.hh"
#include "HPGe_Collection.hh"
#include "LeadShieldingUTR_2021-11-08_to_2021-11-21.hh"
#include "MOLLY.hh"
#include "Mechanical.hh"
#include "ZeroDegreeMechanical.hh"

vector<Detector *> detectors = {
    new HPGe_Clover(
        "clover_1", HPGe_Clover_Collection::HPGe_Clover_Yale, 90. * deg,
        0. * deg, 8.00 * inch,
        {{{"G4_Cu", cu_clover_thin}, {"G4_Pb", pb_clover_thin}}, true}, {},
        0.5 * pi),
    new HPGe_Clover(
        "clover_3", HPGe_Clover_Collection::HPGe_Clover_Yale, 90. * deg,
        90. * deg, 8.00 * inch,
        {{{"G4_Cu", cu_clover_thin}, {"G4_Pb", pb_clover_thin}}, true}, {},
        -0.5 * pi),
    new HPGe_Clover(
        "clover_5", HPGe_Clover_Collection::HPGe_Clover_Yale, 90. * deg,
        180. * deg, 8.00 * inch,
        {{{"G4_Cu", cu_clover_thin}, {"G4_Pb", pb_clover_thin}}, true}, {},
        0.5 * pi),
    new HPGe_Clover(
        "clover_7", HPGe_Clover_Collection::HPGe_Clover_Yale, 90. * deg,
        270. * deg, 8.00 * inch,
        {{{"G4_Cu", cu_clover_thin}, {"G4_Pb", pb_clover_thin}}, true}, {},
        0.5 * pi),
    new HPGe_Clover(
        "clover_B1", HPGe_Clover_Collection::HPGe_Clover_Yale, 135. * deg,
        0. * deg, 8.00 * inch,
        {{{"G4_Cu", cu_clover_thin}, {"G4_Pb", pb_clover_very_thin}}, true}, {},
        0.5 * pi),
    new HPGe_Clover(
        "clover_B3", HPGe_Clover_Collection::HPGe_Clover_Yale, 125.26 * deg,
        90. * deg, 8.00 * inch,
        {{{"G4_Cu", cu_clover_thin}, {"G4_Pb", pb_clover_thin}}, true}, {},
        -0.5 * pi),
    new HPGe_Clover(
        "clover_B5", HPGe_Clover_Collection::HPGe_Clover_Yale, 135. * deg,
        180. * deg, 8.00 * inch,
        {{{"G4_Cu", cu_clover_thin}, {"G4_Pb", pb_clover_thin}}, true}, {},
        -0.5 * pi),

    new CeBr3_2x2("cebr_B", 90. * deg, 27.5 * deg, 8.00 * inch),
    new CeBr3_2x2("cebr_C", 90. * deg, 45. * deg, 8.00 * inch),
    new CeBr3_2x2("cebr_D", 90. * deg, 62.5 * deg, 8.00 * inch),
    new CeBr3_2x2("cebr_F", 90. * deg, 117.5 * deg, 8.00 * inch),
    new CeBr3_2x2("cebr_G", 90. * deg, 135. * deg, 8.00 * inch),
    new CeBr3_2x2("cebr_H", 90. * deg, 152.5 * deg, 8.00 * inch),
    new CeBr3_2x2("cebr_K", 90. * deg, 225. * deg, 8.00 * inch),
    new CeBr3_2x2("cebr_BK", 135. * deg, 21. / 28. * 360. * deg, 8.00 * inch),

    new HPGe_Coaxial("zero_degree",
                     HPGe_Coaxial_Collection::HPGe_120_TUNL_40383,
                     HPGe_Coaxial_Dewar_Properties(), 0. * deg, 0. * deg,
                     ZeroDegreeMechanical::zero_degree_to_target),
    new MOLLY(0. * deg, 0. * deg, 11. * m)};

G4VPhysicalVolume *DetectorConstruction::Construct() {

  ConstructBoxWorld(2. * m, 2. * m, 12.0 * m);

  CollimatorRoom(world_logical).Construct({});
  BeamPipe(world_logical).Construct({});
  LeadShieldingUTR(world_logical).Construct({});
  Mechanical(world_logical).Construct({});
  ZeroDegreeMechanical(world_logical)
      .Construct(G4ThreeVector(zero_degree_x, zero_degree_y, 0.));
  GammaVault(world_logical).Construct({});

  for (size_t n_detector = 0; n_detector < detectors.size() - 2; ++n_detector) {
    detectors[n_detector]->Construct(world_logical, {});
    RegisterSensitiveLogicalVolumes(
        detectors[n_detector]->get_sensitive_logical_volumes());
  }
  detectors[detectors.size() - 2]->Construct(
      world_logical, G4ThreeVector(zero_degree_x, zero_degree_y, 0.));
  RegisterSensitiveLogicalVolumes(
      detectors[detectors.size() - 2]->get_sensitive_logical_volumes());
  detectors[detectors.size() - 1]->Construct(world_logical,
                                             G4ThreeVector(molly_x, 0., 0.));
  RegisterSensitiveLogicalVolumes(
      detectors[detectors.size() - 1]->get_sensitive_logical_volumes());

  return world_phys;
}
