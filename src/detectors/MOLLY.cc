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

#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

#include "MOLLY.hh"

void MOLLY::Construct_Detector(G4LogicalVolume *world_logical,
                               G4ThreeVector global_coordinates) {

  G4NistManager *nist = G4NistManager::Instance();
  const G4ThreeVector e_r = unit_vector_r(theta, phi);

  const double crystal_dimension = 125. * mm;
  G4Tubs *crystal_solid =
      new G4Tubs(detector_name + "_crystal_solid", 0., crystal_dimension,
                 crystal_dimension, 0., twopi);
  sensitive_logical_volumes.push_back(new G4LogicalVolume(
      crystal_solid, nist->FindOrBuildMaterial("G4_SODIUM_IODIDE"),
      detector_name + "_crystal_logical"));
  sensitive_logical_volumes[0]->SetVisAttributes(G4Color::Magenta());
  G4RotationMatrix *rotation = new G4RotationMatrix();
  rotation->rotateZ(-phi);
  rotation->rotateY(-theta);
  new G4PVPlacement(rotation,
                    global_coordinates +
                        (dist_from_center + crystal_dimension) * e_r,
                    sensitive_logical_volumes[0], detector_name + "crystal",
                    world_logical, false, 0, false);
}

G4VSolid *MOLLY::Filter_Shape(const string name, const Filter &filter) const {
  return new G4Tubs(name, 0., filter.radius, filter.thickness * 0.5, 0., twopi);
}