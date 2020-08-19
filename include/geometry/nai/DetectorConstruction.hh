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

    Copyright (C) 2020 Udo Friman-Gayer
*/

#pragma once

#include "NDetectorConstruction.hh"

/**
 * \brief γγ-coincidence spectrometer with two NaI(Tl) detectors
 * 
 * This is a simplified version of the setup described in Refs. \cite Tillett2017 and 
 * \cite Tillett2018.
 * A previous implementation of the spectrometer was provided to the author by J. Dermigny, one 
 * of the authors of the aforementioned publications.
 * From this code, only the essential parts (two NaI(Tl) detectors, sample, sample holder, divider 
 * wall) have been imported.
 * Many comments were removed, and some code was adapted to conform with nutr.
 * Except for the world volume, all dimensions in the Construct() method are exactly the same as in
 * J. Dermigny's code.
 * 
 * The two detectors are implemented in the separate NaI class.
 * See the description of that class for more information.
 */
class DetectorConstruction : public NDetectorConstruction
{
public:
    G4VPhysicalVolume* Construct() override final;
};
