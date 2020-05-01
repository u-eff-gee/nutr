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

#include <cmath>

#include "EulerAngleRotation.hh"

array<double, 3> EulerAngleRotation::rotate(const array<double, 3> x_y_z, const array<double, 3> phi_theta_psi) const {

    const array<array<double, 3>, 3> A = rotation_matrix(phi_theta_psi);

    return array<double, 3>{
        A[0][0]*x_y_z[0] + A[0][1]*x_y_z[1] + A[0][2]*x_y_z[2],
        A[1][0]*x_y_z[0] + A[1][1]*x_y_z[1] + A[1][2]*x_y_z[2],
        A[2][0]*x_y_z[0] + A[2][1]*x_y_z[1] + A[2][2]*x_y_z[2]
    };
}

array<double, 3> EulerAngleRotation::rotate_back(const array<double, 3> xp_yp_zp, const array<double, 3> phi_theta_psi) const {
    return rotate(xp_yp_zp, {-phi_theta_psi[2], -phi_theta_psi[1], -phi_theta_psi[0]});
}

array<double, 2> EulerAngleRotation::get_theta_phi(const array<double, 3> x_y_z_norm) const {
    
    return array<double, 2>{ 
        acos(x_y_z_norm[2]), 
        atan(x_y_z_norm[1]/x_y_z_norm[0]) 
    };

}

array<double, 3> EulerAngleRotation::get_x_y_z_norm(const array<double, 2> theta_phi) const {
    
    double cos_the{cos(theta_phi[0])}, sin_the{sin(theta_phi[0])}, cos_phi{cos(theta_phi[1])}, sin_phi{sin(theta_phi[1])};

    return array<double, 3>{ 
        sin_the*cos_phi,
        sin_the*sin_phi,
        cos_the
    };

}

array<array<double, 3>, 3> EulerAngleRotation::rotation_matrix(const array<double, 3> phi_theta_psi) const {
    
    const double cos_phi{cos(phi_theta_psi[0])}, sin_phi{sin(phi_theta_psi[0])}, cos_the{cos(phi_theta_psi[1])}, sin_the{sin(phi_theta_psi[1])}, cos_psi{cos(phi_theta_psi[2])}, sin_psi{sin(phi_theta_psi[2])};

    return array<array<double, 3>, 3> {
        array<double, 3>{ 
            cos_psi*cos_phi - cos_the*sin_phi*sin_psi,
            cos_psi*sin_phi + cos_the*cos_phi*sin_psi,
            sin_psi*sin_the
        },
        array<double, 3>{
            -sin_psi*cos_phi - cos_the*sin_phi*cos_psi,
            -sin_psi*sin_phi + cos_the*cos_phi*cos_psi,
            cos_psi*sin_the
        },
        array<double, 3>{
            sin_the*sin_phi,
            -sin_the*cos_phi,
            cos_the
        }
    };

}