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

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

/**
 * \brief Plexiglass beam pipe used in the 2020/2021 experiments.
 * 
 * A vacuum pump can be attached to the beam pipe to evacuate the beam line.
 * If the pump is running at maximum power, a value of 28.5(3) inches Hg  was read off on
 * the vacuum gauge, which corresponds to about -96.2(10) hPa or about 4 % of standard air
 * density/pressure.
 * 
 * On the upstream end, the pipe is sealed by a cap.
 * On the downstream end, a pipe with a smaller diameter and an end cap that seals the 
 * beam pipe can be slid in.
 */
class BeamPipe{
public:
    BeamPipe(
        G4LogicalVolume* world_log,
        const double air_rel_dens=0.04,
        const bool use_up_cap=true,
        const bool use_down_pipe=true,
        const bool use_ring=true
        ):
        world_logical(world_log),
        air_relative_density(air_rel_dens),
        use_upstream_cap(use_up_cap),
        use_downstream_pipe(use_down_pipe),
        use_target_ring(use_ring){};
    void Construct(const G4ThreeVector global_coordinates);

protected:
    G4LogicalVolume *world_logical;
    const double air_relative_density;
    const bool use_upstream_cap;
    const bool use_downstream_pipe;
    const bool use_target_ring;
};