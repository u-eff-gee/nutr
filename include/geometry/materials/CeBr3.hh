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

#include "G4Material.hh"

#include "Material.hh"

/**
 * \brief Cerium bromide scintillator material.
 *
 * Uses the standard properties from Wikipedia \cite CeriumBromideWikipedia2020.
 */
class CeBr3 : public Material {
public:
  CeBr3();
};