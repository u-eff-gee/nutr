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

#include "ComptonMonitor_2021-04-19_to_2021-04-30.hh"
#include "ComptonMonitorTarget.hh"

void ComptonMonitor::Construct(const G4ThreeVector global_coordinates) {

  // Scattering target
  ComptonMonitorTarget compton_monitor_target(world_logical);
  compton_monitor_target.Construct(
      global_coordinates + G4ThreeVector(0., 0., scattering_target_to_target));
}