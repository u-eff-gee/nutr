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
 * \brief Clover Array @ HIγS in the spring-2021 campaign, part I.
 *
 * This includes the following experiments:
 * PI                 - Target      - First run - Last run - Comment
 * Janssens             62Ni/78Zn     51          621
 * Kleemann/Pietralla   154Sm/140Ce   622         633        Test runs
 * Finch                56Fe          634         637        Test runs
 */
class DetectorConstruction : public NDetectorConstruction {
public:
  G4VPhysicalVolume *Construct() override final;
};
