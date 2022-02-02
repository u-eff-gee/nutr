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

#include <memory>

using std::make_shared;

#include "LaBr3Ce.hh"

#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"

LaBr3Ce::LaBr3Ce(){
    material = make_shared<G4Material>("LaBr3Ce", 5.08*g/cm3, 3);
	G4NistManager *nist = G4NistManager::Instance();
    material->AddElement(nist->FindOrBuildElement("La"), 95);
    material->AddElement(nist->FindOrBuildElement("Br"), 300);
    material->AddElement(nist->FindOrBuildElement("Ce"), 5);
}