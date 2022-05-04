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

#include "SourceVolume.hh"

SourceVolume::SourceVolume(G4VSolid *solid, G4VPhysicalVolume *physical,
                           const double rel_int)
    : source_solid(solid), source_physical(physical),
      relative_intensity(rel_int) {}

void SourceVolume::initialize(const int seed) { random_engine = mt19937(seed); }