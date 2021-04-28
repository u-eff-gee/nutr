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

	Copyright (C) 2020, 2021 Udo Friman-Gayer
*/

#pragma once

#include <memory>

using std::shared_ptr;

#include "G4Material.hh"

/**
 * \brief Abstract class for a user-defined material.
 * 
 * This class exists to simplify the registration of custom materials in Geant4.
 * As soon as the constructor of G4Material is called, the new material is registered internally
 * in the G4MaterialTable, which is simply a std::vector of all defined materials.
 * This has the advantage that materials are available globally.
 * 
 * In an earlier implementation of nutr, a class for scintillation detectors defined a new material
 * inside its Construct() method.
 * If several of these detectors were needed in a geometry, this caused warnings saying that the
 * material had already been defined.
 * 
 * To prevent these warnings, this abstract class was defined.
 * User-defined materials should inherit from Materials and implement a constructor that 
 * initializes Materials::material.
 * In order to use the material in the geometry, the class has to be forward-declared.
 */
class Material{

protected:
    shared_ptr<G4Material> material; /**< Pointer to the material implemented in this class. */
};