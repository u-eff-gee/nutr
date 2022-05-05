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

#include "G4Material.hh"

#include "Material.hh"

/**
 * \brief Polyactic acid (PLA).
 *
 * This material is commonly used in 3D printing.
 * Its chemical formula is (C_3 H_4 O_2)_n [1].
 * In the Clover Array setup, it was used for the filter cases of the clover
 * detectors. Since a 3D printed object is usually not made of solid material,
 * we determined the density by weighing a real-life sample and dividing by the
 * volume of the Geant4 model.
 *
 * [1] https://en.wikipedia.org/wiki/Polylactic_acid, accessed on 05/04/2020
 */
class PLA : public Material {
public:
  PLA();
};