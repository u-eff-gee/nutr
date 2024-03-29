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
 * \brief Clover Array @ HIγS in the fall-2021 campaign, part III
 *
 * This includes the following experiments
 * PI                 - Target      - First run - Last run - Comment
 * Papst                96Mo          1           67         low energy
 * Müscher              112Cd         68          134        low energy
 *                                    135         154        radioactive-source
 * calibration Papst                96Mo          155         244        high
 * energy 11B           245         246        calibration 247         249
 * radioactive-source calibration Müscher              112Cd         250 325
 * high energy 32S           326         332        calibration 333         337
 * radioactive-source calibration
 */

#pragma once

#include <string>

class DetectorConstruction : public NDetectorConstruction {
public:
  G4VPhysicalVolume *Construct() override final;
};
