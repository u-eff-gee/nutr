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

#include "NDetectorConstruction.hh"

/**
 * \brief Clover Array @ HIγS in the fall-2021 campaign, part II
 *
 * This includes the following experiment
 * PI                 - Target      - First run - Last run - Comment
 * Banu                 78Kr          860         1028
 */

#pragma once

#include <string>

struct DetectorPosition {
  const std::string id;
  const double theta;
  const double phi;
  const double distance;
  const double intrinsic_rotation_angle;
  const double filter_pb = 0.;
  const double filter_cu = 0.;
};

class DetectorConstruction : public NDetectorConstruction {
public:
  G4VPhysicalVolume *Construct() override final;
};
