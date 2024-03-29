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

#include <string>

using std::string;

#include "G4VUserActionInitialization.hh"

class B4DetectorConstruction;

class ActionInitialization : public G4VUserActionInitialization {
public:
  ActionInitialization(const string out_file_name, const long seed);
  virtual ~ActionInitialization();

  virtual void BuildForMaster() const;
  virtual void Build() const;

private:
  const long random_number_seed;
  const string output_file_name;
};
