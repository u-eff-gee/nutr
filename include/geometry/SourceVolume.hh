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

    Copyright (C) 2020-2022 Udo Friman-Gayer
*/

#pragma once

#include <memory>
#include <random>
#include <vector>

using std::mt19937;
using std::shared_ptr;
using std::uniform_real_distribution;
using std::unique_ptr;

#include "G4VPhysicalVolume.hh"
#include "G4VSolid.hh"

class SourceVolume {
public:
  SourceVolume(G4VSolid *solid, G4VPhysicalVolume *physical,
               const double rel_int);

  virtual G4ThreeVector operator()() = 0;
  double get_relative_intensity() const { return relative_intensity; }
  void initialize(const int seed);

protected:
  shared_ptr<G4VSolid> source_solid;
  unique_ptr<G4VPhysicalVolume> source_physical;

  mt19937 random_engine; /**< Deterministic random number engine. */
  uniform_real_distribution<double>
      uniform_random; /**< Uniform distribution from which all random numbers
                         are sampled here. */

  const double relative_intensity;
};
