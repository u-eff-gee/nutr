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

#pragma once

#include "Detector.hh"

/**
 * @brief MOLLY, large volume sodium-iodide (NaI) detector at HIγS for beam
 * diagnostics
 *
 * This class contains a crude implementation of the MOLLY detector.
 * At the moment, the detector is simply a 25 cm x 25 cm cylinder of NaI without
 * the aluminium case due to lack of a technical drawing. At typical HIγS beam
 * energies of > 2 MeV, the impact of the missing case should be negligible.
 *
 * The dimensions of the detector were taken from:
 *
 * X. Li, Compton Scattering and Nucleon Polarizabilities at HIγS, PhD thesis,
 * Duke University (2020)
 * https://webhome.phy.duke.edu/~mep/thesis/Thesis_XiaqingLi-Dec2020.pdf
 */

class MOLLY : public Detector {
public:
  MOLLY(const double _theta, const double _phi, const double _dist_from_center)
      : Detector("molly", _theta, _phi, _dist_from_center, {{}}, {}, 0.,
                 {0.}){};

protected:
  void Construct_Detector(G4LogicalVolume *world_logical,
                          G4ThreeVector global_coordinates) override final;
  void Construct_Filter_Case(
      [[maybe_unused]] G4LogicalVolume *world_logical,
      [[maybe_unused]] G4ThreeVector global_coordinates,
      [[maybe_unused]] double filter_dist_from_center) override final{};
  G4VSolid *Filter_Shape(const string name,
                         const Filter &filter) const override final;
};