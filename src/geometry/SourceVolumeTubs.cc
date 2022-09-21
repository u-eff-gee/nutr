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

using std::dynamic_pointer_cast;

#include "G4PhysicalConstants.hh"

#include "SourceVolumeTubs.hh"

SourceVolumeTubs::SourceVolumeTubs(G4Tubs *tubs, G4VPhysicalVolume *physical,
                                   const double rel_int)
    : SourceVolume(tubs, physical, rel_int) {}

G4ThreeVector SourceVolumeTubs::operator()() {

  shared_ptr<G4Tubs> source_tubs = dynamic_pointer_cast<G4Tubs>(source_solid);

  const double min_r =
      (source_tubs->GetInnerRadius() * source_tubs->GetInnerRadius()) /
      (source_tubs->GetOuterRadius() * source_tubs->GetOuterRadius());

  double random_r = source_tubs->GetOuterRadius() *
                    sqrt(min_r + (1. - min_r) * uniform_random(random_engine));
  double random_phi =
      source_tubs->GetStartPhiAngle() +
      uniform_random(random_engine) * source_tubs->GetDeltaPhiAngle();
  double random_z =
      (2. * uniform_random(random_engine) - 1.) * source_tubs->GetZHalfLength();

  G4RotationMatrix rotation;
  if (source_physical->GetRotation()) {
    rotation = *(source_physical->GetRotation());
  }
  auto translation = source_physical->GetTranslation();

  auto res = G4ThreeVector(random_r * cos(random_phi),
                           random_r * sin(random_phi), random_z)
                 .transform(rotation) +
             translation;

  return res;
}
