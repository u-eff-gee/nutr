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

#include "DetectorConstructionMessenger.hh"

class DetectorConstructionMessenger;

#include "NDetectorConstruction.hh"

/**
 * \brief Clover Array @ HIγS in the spring-2021 campaign, part III.
 *
 * This includes the following experiments:
 * PI                 - Target      - First run - Last run - Comment
 * Kleemann/Pietralla   140Ce/154Sm   699         702        Test runs
 * Kleemann/Pietralla   140Ce/154Sm   707         805
 */
class DetectorConstruction : public NDetectorConstruction {
public:
  DetectorConstruction();
  G4VPhysicalVolume *Construct() override final;

  void activation_target_in_out(const bool in) {
    in ? use_activation_target = true : use_activation_target = false;
  }

protected:
  DetectorConstructionMessenger *messenger;

  bool use_activation_target;
};
