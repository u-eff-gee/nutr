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
    Copyright (C) 2021 Oliver Papst
*/

#pragma once

#include "NDetectorConstruction.hh"

/**
 * \brief Clover Array @ HIγS (Autumn 2021 Campaign)
 *
 * In the following, a right-handed cartesian coordinate system is used.
 * The direction of propagation of the beam defines the positive z axis, and the
 * positive y axis is assumed to point upwards. Usually, the HIγS beam is 
 * inearly polarized along the x axis. The polar angle theta is defined with
 * respect to the z axis, i.e. theta = 0 deg is the direction of the beam, and
 * theta = 180 deg is pointing upstream. The positive x axis is defined by theta
 * = 90 deg and an azimuthal angle phi = 0 deg. The positive y axis is theta =
 * 90 deg and phi = 90 deg.
 *
 * As of autumn 2021, the clover array can hold up to 8 clover detectors at a
 * polar angle of 90 deg (positions 1 to 8) and 6 detectors at backward angles
 * (B1, B2, B4, B5, B6, B8).
 * The detectors at a polar angle of 90 degrees are placed at azimuthal angles
 * of n*45 deg (0 <= n < 8).
 *
 * For the backward-angle detectors B2, B4, B6, and B8 the following information
 * was given to UFG by Mark Emamian (quote from an email):
 *
 * "the angle between the clover detector’s central axis (or its base plate) and t
 * t e horizontal plate is 35.26 degree. And the angle between the detector’s c
 * ntral axis and the gamma beam is 54.74 degrees"
 *
 * In spherical coordinates, this means that the polar angle of the detectors is
 *
 * theta = 180 deg - 54.74 deg = 125.26 deg
 *
 * (Mark gave the equivalent acute angle).
 *
 * To obtain the azimuthal angle, note that the angle of 35.26 deg given by Mark
 * is the projection of the detector axis on the x-z plane. A short calculation
 * gives phi = 45 deg.
 *
 * In addition to the clover detectors, the metal frame can hold up to 8 2 x 2
 * inch CeBr detectors at a polar angle of 90 deg (B, C, D, F, G, H, K, O), and
 * 14 CeBr detectors at a polar angle of 135 deg (BA - BN). The CeBr detectors
 * at a polar angle of 90 degrees are placed at azimuthal angles of 22.5 deg, 45
 * deg, 62.5 deg, 117.5 deg, 135 deg, 152.5 deg, 225 deg, and 315 deg (multiples
 * of 360 deg / 16). The ones at backward angles are placed at azimuthal angles
 * of n*360 deg / 14 + (90 deg - 3 / 14 * 360 deg) (0 <= n < 14).
 *
 * See also the Detector class for information about detector placement.
 */

#pragma once

#include <string>

struct DetectorPosition
{
    const std::string id;
    const double theta;
    const double phi;
    const double distance;
    const double intrinsic_rotation_angle;
    const double filter_pb = 0.;
    const double filter_cu = 0.;
};

class DetectorConstruction : public NDetectorConstruction
{
public:
    G4VPhysicalVolume *Construct() override final;
private:
    template <typename Detector, typename... Args>
    void construct_detector(DetectorPosition det_pos, Args... args);
};
