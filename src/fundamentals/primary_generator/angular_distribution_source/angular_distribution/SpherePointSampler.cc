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
#include <cmath>
#include <stdexcept>

#include <gsl/gsl_sf_ellint.h>
#include <gsl/gsl_sf_elljac.h>

#include "SpherePointSampler.hh"

using std::array;
using std::invalid_argument;

double SpherePointSampler::elliptic_integral_2nd_kind_arbitrary_m(const double phi, const double m) const {

    const double abs_m = fabs(m);

    // Use Eq. (19.6.9) in Ref. \cite DLMF2020.
    // The GSL implementation cannot handle this case.
    if(m == 1.){
        return sin(phi);
    }

    const double abs_k = sqrt(abs_m);

    // Direct call of the GSL library function possible.
    if(m >= 0. && abs_m < 1.){
            return gsl_sf_ellint_E(phi, abs_k, GSL_PREC_DOUBLE);
    }

    // Use Eq. (19.7.5) in Ref. \cite DLMF2020.
    const double k_squared_plus_one = 1.+abs_k*abs_k;
    const double kappa_prime = 1./sqrt(k_squared_plus_one);
    const double kappa = abs_k*kappa_prime;
    const double kappa_squared = kappa*kappa;
    const double theta = asin(
        sin(phi)
        /(kappa_prime*sqrt(1.+abs_k*abs_k*pow(sin(phi), 2)))
    );

    return 1./kappa_prime*(
        elliptic_integral_2nd_kind_arbitrary_m(theta, kappa_squared)
        -kappa_squared*
            sin(theta)*cos(theta)
            /sqrt(1.-kappa_squared*pow(sin(theta), 2))
    );

    // Alternative implementation of the cases \f$m < 0\f$ and \f$ |m| > 1 \f$ based on 
    // Eqs. (17.4.16) and (17.4.18) in 
    // \cite AbramowitzStegun1974, which did not give the correct results.
    // Possibly something went wrong in the transformation between \f$\varphi\f$ and \f$u\f$.
    //
    // const double u = elliptic_integral_1st_kind_arbitrary_m(phi, m);
    // 
    // if(m < 0.){
    //     const double sqrt_1_plus_abs_m = sqrt(1. + abs_m);
    //     const double abs_m_over_1_plus_abs_m = abs_m/(1. + abs_m);

    //     double sn_u_times_sqrt_1_plus_abs_m, 
    //     cn_u_times_sqrt_1_plus_abs_m, dn_u_times_sqrt_1_plus_abs_m;

    //     gsl_sf_elljac_e(u*sqrt_1_plus_abs_m, abs_m_over_1_plus_abs_m, &sn_u_times_sqrt_1_plus_abs_m, &cn_u_times_sqrt_1_plus_abs_m, &dn_u_times_sqrt_1_plus_abs_m);

    //     return sqrt_1_plus_abs_m*(
    //         elliptic_integral_2nd_kind_arbitrary_m(
    //             asin(sn_u_times_sqrt_1_plus_abs_m),
    //             abs_m_over_1_plus_abs_m)
    //         -abs_m/sqrt_1_plus_abs_m*sn_u_times_sqrt_1_plus_abs_m*cn_u_times_sqrt_1_plus_abs_m/dn_u_times_sqrt_1_plus_abs_m
    //     );
    // }

    // const double inverse_abs_m = 1./abs_m;

    // double sn_u_times_sqrt_abs_m, cn_u_times_sqrt_abs_m, dn_u_times_sqrt_abs_m;

    // gsl_sf_elljac_e(u*sqrt_abs_m, inverse_abs_m, &sn_u_times_sqrt_abs_m, &cn_u_times_sqrt_abs_m, &dn_u_times_sqrt_abs_m);

    // return sqrt_abs_m*elliptic_integral_2nd_kind_arbitrary_m(
    //     asin(sn_u_times_sqrt_abs_m),
    //     inverse_abs_m)
    // -(abs_m-1.)*u;

}

double SpherePointSampler::segment_length(const double Theta, const double c) const {
    if(Theta >= 0 && Theta <= M_PI_2){
        return elliptic_integral_2nd_kind_arbitrary_m(Theta, -c*c);
    }
    if(Theta <= M_PI){
        return 2.*elliptic_integral_2nd_kind_arbitrary_m(M_PI_2, -c*c)
        - elliptic_integral_2nd_kind_arbitrary_m(M_PI - Theta, -c*c);
    }
}

double SpherePointSampler::segment_length_linear_interpolation(const double Theta, const double c, const unsigned int n_points) const {
    const double theta_increment = Theta/((double) n_points - 1.);
    array<double, 3> x_i{0., 0., 1.};
    array<double, 3> x_i_plus_one{0., 0., 0.};
    double phi_i_plus_one{0.}, segment_increment_squared{0.}, segment_length{0.}, sine_theta_i_plus_one{0.}, theta_i_plus_one{0.};

    for(unsigned int i = 0; i < n_points-1; ++i){
        theta_i_plus_one = (i+1)*theta_increment;
        sine_theta_i_plus_one = sin(theta_i_plus_one);
        phi_i_plus_one = c*theta_i_plus_one;

        x_i_plus_one[0] = sine_theta_i_plus_one*cos(phi_i_plus_one);
        x_i_plus_one[1] = sine_theta_i_plus_one*sin(phi_i_plus_one);
        x_i_plus_one[2] = cos(theta_i_plus_one);

        segment_increment_squared = 0;
        for(size_t j = 0; j < 3; ++j){
            segment_increment_squared += (x_i_plus_one[j] - x_i[j])*(x_i_plus_one[j] - x_i[j]);
        }

        segment_length += sqrt(segment_increment_squared);

        for(size_t j = 0; j < 3; ++j){
            x_i[j] = x_i_plus_one[j];
        }
    }

    return segment_length;
}