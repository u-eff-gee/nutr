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
#include <sstream>
#include <stdexcept>

#include <gsl/gsl_sf_ellint.h>
#include <gsl/gsl_sf_elljac.h>

#include "SpherePointSampler.hh"

using std::invalid_argument;
using std::runtime_error;
using std::stringstream;

array<vector<double>, 2> SpherePointSampler::sample(const unsigned int n) const {
    array<vector<double>, 2> theta_phi = {vector<double>(n, 0.), vector<double>(n, 0.)};

    const double c = find_c(n);

    for(size_t j = 1; j <= n; ++j){
        theta_phi[0][j] = find_Theta_j(j, n, c);
        theta_phi[1][j] = c*theta_phi[0][j];
    }

    return theta_phi;
}

array<vector<double>, 3> SpherePointSampler::sample_cartesian(const unsigned int n, const double r) const {

    const array<vector<double>, 2> theta_phi = sample(n);
    
    array<vector<double>, 3> x_y_z = {vector<double>(n, 0.), vector<double>(n, 0.), vector<double>(n, 0.)};

    double sine_theta = 0.;

    for(size_t i = 0; i < n; ++i){
        sine_theta = sin(theta_phi[0][i]);
        x_y_z[0][i] = r*sine_theta*cos(theta_phi[1][i]);
        x_y_z[1][i] = r*sine_theta*sin(theta_phi[1][i]);
        x_y_z[2][i] = r*cos(theta_phi[0][i]);
    }

    return x_y_z;
}

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
    const double abs_k_squared = abs_k*abs_k;
    const double kappa_prime = 1./sqrt(1. + abs_k_squared);
    const double kappa = abs_k*kappa_prime;
    const double kappa_squared = kappa*kappa;
    // phi == pi/2 corresponds to the complete elliptic integral
    // This is both a shortcut and an insurance against numerical instabilities.
    if(phi == M_PI_2){
        return gsl_sf_ellint_Ecomp(kappa, GSL_PREC_DOUBLE)/kappa_prime;
    }

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
        const double negative_c_squared = -c*c;
        return 2.*elliptic_integral_2nd_kind_arbitrary_m(M_PI_2, negative_c_squared)
        - elliptic_integral_2nd_kind_arbitrary_m(M_PI - Theta, negative_c_squared);
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

double SpherePointSampler::find_c(const unsigned int n, const double epsilon, const unsigned int max_n_iterations) const {

    if(n < 2){
        throw invalid_argument("Number of points smaller than 2 requested.");
    }

    // Initial guess from Ref. \cite Koay2011
    const double c_0 = sqrt(n*M_PI);
    double c_j = c_0;
    double c_j_plus_one = 0.;
    double negative_c_j_squared = 0;
    double complete_elliptic_integral_1st{0.}, complete_elliptic_integral_2nd{0.};

    const double n_times_pi = n*M_PI;

    for(unsigned int i = 0; i < max_n_iterations; ++i){
        negative_c_j_squared = -c_j*c_j;
        complete_elliptic_integral_1st = elliptic_integral_1st_kind_arbitrary_m(M_PI_2, negative_c_j_squared);
        complete_elliptic_integral_2nd = elliptic_integral_2nd_kind_arbitrary_m(M_PI_2, negative_c_j_squared);

        c_j_plus_one =
            (c_j*n_times_pi*(
                    2.*complete_elliptic_integral_2nd 
                    - complete_elliptic_integral_1st
                ))
                /(
                    n_times_pi*complete_elliptic_integral_2nd
                    -n_times_pi*complete_elliptic_integral_1st
                    +c_j*complete_elliptic_integral_2nd*complete_elliptic_integral_2nd
                );

        if(fabs(c_j_plus_one - c_j) < epsilon){
            return c_j_plus_one;
        }

        c_j = c_j_plus_one;
    }
    
    stringstream error_message;
    error_message << "No value for c found after " << max_n_iterations << " iterations with an initial value of c_0 = " << c_0;

    throw runtime_error(error_message.str());
}

double SpherePointSampler::find_Theta_j(const unsigned int j, const unsigned int n, const double c, const double epsilon, const unsigned int max_n_iterations) const {

    if(j == 0 || j > n){

        stringstream error_message;
        error_message << "j must be an nonzero positive integer between 1 and n = " << n;

        throw invalid_argument(error_message.str());
    }

    // Initial guess from Ref. \cite Koay2011
    // Note that j is fixed here, and the iteration index is denoted as l
    const double Theta_j_0 = acos(1.-(2.*j-1.)/(double) n);
    double Theta_j_l = Theta_j_0;

    double Theta_j_l_plus_one = 0.;

    for(unsigned int i = 0; i < max_n_iterations; ++i){
        Theta_j_l_plus_one = Theta_j_l
            +((2.*j - 1.)*M_PI - c*segment_length(Theta_j_l, c))
            /(c*sqrt(1.+c*c*pow(sin(Theta_j_l), 2)));

        if(fabs(Theta_j_l_plus_one - Theta_j_l) < epsilon){
            return Theta_j_l_plus_one;
        }

        Theta_j_l = Theta_j_l_plus_one;
    }

    stringstream error_message;
    error_message << "No value for Theta_j found after " << max_n_iterations << " iterations with an initial value of Theta_j_0 = " << Theta_j_0;

    throw runtime_error(error_message.str());
}

double SpherePointSampler::elliptic_integral_1st_kind_arbitrary_m(const double phi, const double m) const {

    const double abs_m = fabs(m);
    const double abs_k = sqrt(abs_m);

    if(m >= 0. && abs_m < 1.){
        return gsl_sf_ellint_F(phi, abs_k, GSL_PREC_DOUBLE);
    }

    // Use Eq. (19.7.5) in Ref. \cite DLMF2020.
    const double abs_k_squared = abs_k*abs_k;
    const double kappa_prime = 1./sqrt(1. + abs_k_squared);
    const double kappa = abs_k*kappa_prime;

    // phi == pi/2 corresponds to the complete elliptic integral
    // This is both a shortcut and an insurance against numerical instabilities.
    if(phi == M_PI_2){
        return kappa_prime*gsl_sf_ellint_Kcomp(kappa, GSL_PREC_DOUBLE);
    }
    
    const double theta = asin(
        sin(phi)
        /(kappa_prime*sqrt(1.+abs_k_squared*pow(sin(phi), 2)))
    );

    return kappa_prime*elliptic_integral_1st_kind_arbitrary_m(theta, kappa*kappa);

}
