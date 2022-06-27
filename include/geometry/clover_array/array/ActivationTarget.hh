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

#include "G4LogicalVolume.hh"

/**
 * @brief Target holder for photoactivation experiments.
 *
 * This class implements three parts:
 *
 * 1) The acrylic capsule that contains the ideally disk-shaped activation
 * targets and probably some filling material.
 *
 * 2) The polyethylene base that
 * holds the capsule.
 *
 * 3) The aluminium rod that holds the base and fits into
 * standard translation stages for optics experiments.
 *
 * The capsule is the only part that is not completely symmetric, because the
 * lid is 15% thicker than the bottom (probably to fit the screw heads exactly,
 * which are not implemented here). It is assumed here that the bottom faces
 * upstream, because photographs from actual experiments show this orientation,
 * and the experimentalists usually made sure to position the target exactly the
 * same way.
 *
 * Original implementation of some parts by J. Kleemann as part of the `utr`
 * code
 *
 * https://github.com/uga-uga/utr
 */
class ActivationTarget {
public:
  ActivationTarget(G4LogicalVolume *_world_logical)
      : world_logical(_world_logical) {}
  void Construct(const G4ThreeVector global_coordinates);

protected:
  G4LogicalVolume *world_logical;
};