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

#include "G4Material.hh"

#include "Material.hh"

/**
 * \brief Cerium-doped lanthanum bromide material.
 * 
 * The composition used here corresponds to the "BrilLanCe 380" trademark material 
 * \cite BrilLanCe380SaintGobain2021 \cite BrilLanCe380gammadata2021.
 * In Ref. \cite BrilLanCe380SaintGobain, the percentage of Ce is given as 5%.
 * It was assumed that this is the percentage of lanthanum atoms that have been replaced by cerium.
 * In Ref. \cite BrilLanCe380gammadata2021, the density of the material is given as 5.08 g/cm3.
 * From this information and the known chemical formula, the relative contents of the constituents 
 * were deduced (see the implementation of the constructor).
 */
class LaBr3Ce : public Material{
public:
    LaBr3Ce();
};