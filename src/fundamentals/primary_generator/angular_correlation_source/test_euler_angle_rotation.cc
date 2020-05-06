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

#include <array>

#include <gsl/gsl_math.h>

#include "EulerAngleRotation.hh"
#include "TestUtilities.hh"

/**
 * \brief Test rotations of 3D vectors using Euler angles.
 * 
 * Test by rotating the three canonical Cartesian axes into each other.
 */
int main(){

    const double epsilon = 1e-8;

    EulerAngleRotation eul_ang_rot;

    array<double, 3> x_axis{1., 0., 0.};
    array<double, 2> x_axis_sph{M_PI_2, 0.};
    array<double, 3> y_axis{0., 1., 0.};
    array<double, 2> y_axis_sph{M_PI_2, M_PI_2};
    array<double, 3> z_axis{0., 0., 1.};
    // Warning: On the z axis, the angle phi in spherical coordinates is actually undefined.
    // Therefore, a test in which, after a rotation into the z axis, Cartesian coordinates are
    // converted back into spherical coordinates, may not result in the coordinates theta = 0,
    // phi =0.
    // This is taken into account in the tests by only requiring that the value of theta is 
    // (numerically close to) zero.
    array<double, 2> z_axis_sph{0., 0.};

    array<double, 3> euler_angles{0., 0., 0.};
    array<double, 3> xp_yp_zp{0., 0., 0.};
    array<double, 2> thetap_phip{0., 0.};

    // Rotate x axis into y axis
    // Phi   = -pi/2
    // Theta = 0
    // Psi   = 0
    euler_angles = {-M_PI_2, 0., 0.};

    xp_yp_zp = eul_ang_rot.rotate(x_axis, euler_angles);
    test_numerical_equality<double>(3, xp_yp_zp.data(), y_axis.data(), epsilon);
    test_numerical_equality<double>(3, eul_ang_rot.rotate_back(xp_yp_zp, euler_angles).data(), x_axis.data(), epsilon);

    thetap_phip = eul_ang_rot.rotate(x_axis_sph, euler_angles);
    test_numerical_equality<double>(2, thetap_phip.data(), y_axis_sph.data(), epsilon);
    test_numerical_equality<double>(2, eul_ang_rot.rotate_back(thetap_phip, euler_angles).data(), x_axis_sph.data(), epsilon);

    // Rotate x axis into z axis
    // Phi   = pi/2
    // Theta = pi/2
    // Psi   = 0
    euler_angles = {M_PI_2, M_PI_2, 0.};

    xp_yp_zp = eul_ang_rot.rotate(x_axis, euler_angles);
    test_numerical_equality<double>(3, xp_yp_zp.data(), z_axis.data(), epsilon);
    test_numerical_equality<double>(3, eul_ang_rot.rotate_back(xp_yp_zp, euler_angles).data(), x_axis.data(), epsilon);

    thetap_phip = eul_ang_rot.rotate(x_axis_sph, euler_angles);
    test_numerical_equality<double>(2, thetap_phip.data(), z_axis_sph.data(), epsilon);
    test_numerical_equality<double>(2, eul_ang_rot.rotate_back(thetap_phip, euler_angles).data(), x_axis_sph.data(), epsilon);

    // Rotate y axis into x axis
    // Phi   = pi/2
    // Theta = 0
    // Psi   = 0
    euler_angles = {M_PI_2, 0., 0.};

    xp_yp_zp = eul_ang_rot.rotate(y_axis, euler_angles);
    test_numerical_equality<double>(3, xp_yp_zp.data(), x_axis.data(), epsilon);
    test_numerical_equality<double>(3, eul_ang_rot.rotate_back(xp_yp_zp, euler_angles).data(), y_axis.data(), epsilon);

    thetap_phip = eul_ang_rot.rotate(y_axis_sph, euler_angles);
    test_numerical_equality<double>(2, thetap_phip.data(), x_axis_sph.data(), epsilon);
    test_numerical_equality<double>(2, eul_ang_rot.rotate_back(thetap_phip, euler_angles).data(), y_axis_sph.data(), epsilon);

    // Rotate y axis into z axis
    // Phi   = 0
    // Theta = -pi/2
    // Psi   = 0
    euler_angles = {0., -M_PI_2, 0.};

    xp_yp_zp = eul_ang_rot.rotate(y_axis, euler_angles);
    test_numerical_equality<double>(3, xp_yp_zp.data(), z_axis.data(), epsilon);
    test_numerical_equality<double>(3, eul_ang_rot.rotate_back(xp_yp_zp, euler_angles).data(), y_axis.data(), epsilon);

    thetap_phip = eul_ang_rot.rotate(y_axis_sph, euler_angles);
    test_numerical_equality<double>(2, thetap_phip.data(), z_axis_sph.data(), epsilon);
    test_numerical_equality<double>(2, eul_ang_rot.rotate_back(thetap_phip, euler_angles).data(), y_axis_sph.data(), epsilon);

    // From here on, note the special role of the z axis in spherical coordinates which leads
    // to arbitrary values for phi.

    // Rotate z axis into x axis
    // Phi   = 0
    // Theta = pi/2
    // Psi   = pi/2
    euler_angles = {0., M_PI_2, M_PI_2};

    xp_yp_zp = eul_ang_rot.rotate(z_axis, euler_angles);
    test_numerical_equality<double>(3, xp_yp_zp.data(), x_axis.data(), epsilon);
    test_numerical_equality<double>(3, eul_ang_rot.rotate_back(xp_yp_zp, euler_angles).data(), z_axis.data(), epsilon);

    thetap_phip = eul_ang_rot.rotate(z_axis_sph, euler_angles);
    test_numerical_equality<double>(2, thetap_phip.data(), x_axis_sph.data(), epsilon);
    test_numerical_equality<double>(eul_ang_rot.rotate_back(thetap_phip, euler_angles)[0], z_axis_sph[0], epsilon);

    // Rotate z axis into y axis
    // Phi   = 0
    // Theta = pi/2
    // Psi   = 0
    euler_angles = {0., M_PI_2, 0.};

    xp_yp_zp = eul_ang_rot.rotate(z_axis, euler_angles);
    test_numerical_equality<double>(3, xp_yp_zp.data(), y_axis.data(), epsilon);
    test_numerical_equality<double>(3, eul_ang_rot.rotate_back(xp_yp_zp, euler_angles).data(), z_axis.data(), epsilon);
    
    thetap_phip = eul_ang_rot.rotate(z_axis_sph, euler_angles);
    test_numerical_equality<double>(2, thetap_phip.data(), y_axis_sph.data(), epsilon);
    test_numerical_equality<double>(eul_ang_rot.rotate_back(thetap_phip, euler_angles)[0], z_axis_sph[0], epsilon);

    // Rotate z axis into z axis (trivial)
    // Phi   = 0
    // Theta = 0
    // Psi   = 0
    euler_angles = {0., 0., 0.};

    xp_yp_zp = eul_ang_rot.rotate(z_axis, euler_angles);
    test_numerical_equality<double>(3, xp_yp_zp.data(), z_axis.data(), epsilon);
    test_numerical_equality<double>(3, eul_ang_rot.rotate_back(xp_yp_zp, euler_angles).data(), z_axis.data(), epsilon);

    thetap_phip = eul_ang_rot.rotate(z_axis_sph, euler_angles);
    test_numerical_equality<double>(thetap_phip[0], z_axis_sph[0], epsilon);
    test_numerical_equality<double>(eul_ang_rot.rotate_back(thetap_phip, euler_angles)[0], z_axis_sph[0], epsilon);

}