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

#include <cmath>

#include <gsl/gsl_math.h>
#include <gsl/gsl_sf.h>

#include "State.hh"
#include "TestUtilities.hh"
#include "Transition.hh"
#include "W_pol_dir.hh"

/**
 * \brief Test polarization-direction correlation
 * 
 * The first test uses the analytical expressions for the angular distribution of a 
 * \f$0^+ \to 1^\pm \to 0^+\f$ sequence, which is a very common case in nuclear resonance 
 * fluorescence experiments.
 *
 * The expression is {Eq. (1) in \cite Pietralla2001}:
 * 
 * \f[
 * 		W\left( \theta \right) = \frac{3}{4} \left[ 1 + \cos^2 \left( \theta \right) \right].
 * \f]
 * 
 * The second test reproduces a more complicated example by Biedenharn 
 * {Eqs. (88) - (89c) in Ref. \cite AjzenbergSelove1960}.
 */ 

// Eq. (1) in Ref. \cite Pietralla2001.
double w_pol_dir_0_1_0(const double theta, const double phi, const EMCharacter em){

	int parity_sign{1};
	if(em == electric){
		parity_sign = -1;
	}

	return 1.+0.5*(
		gsl_sf_legendre_Pl(2, cos(theta))
		+0.5*parity_sign*cos(2.*phi)*gsl_sf_legendre_Plm(2, 2, cos(theta))
	);
}
/**
 * Eqs. (88c) and (89c) in Ref. \cite AjzenbergSelove1960.
 * Note that these expression are not normalized to \f$4 \pi\f$.
 * Furthermore, the angle \f$\varphi\f$ is defined in a different way by /Biedenharn compared 
 * to the convention which is used in the present implementation.
 * 
 * Immediately above Eq. (83) in Ref. \cite AjzenbergSelove1960, Biedenharn first defines 
 * 'the polar angle between the photon's direction of motion and its polarization (Poincare) 
 * vector'.
 * After that, it is stated that '\f$\varphi = 0\f$ is taken to mean that the Poincare vector lies 
 * in the plane of the directions of motion.'.
 * 
 * In Fig. 1 of Ref. \cite FaggHanna1959, on the other hand, it can be seen that \f$\varphi = 0\f$ 
 * is the axis perpendicular to the direction of the emitted radiation.
 * For this reason, the functions will be evaluated at \f$90^\circ - \varphi\f$ below.
 */
double Wa(const double theta, const double phi, const double delta){

	const double cosine_theta = cos(theta);

	return (1. + delta*delta)
	+(0.05714+delta*0.22132)
	*(gsl_sf_legendre_Pl(2, cosine_theta)-0.5*gsl_sf_legendre_Plm(2, 2, cosine_theta)*cos(2.*phi));
}

double Wb(const double theta, const double phi, const double delta){

	const double cosine_theta = cos(theta);

	return (1. + delta*delta)
	+(0.05714+delta*0.22132)
	*gsl_sf_legendre_Pl(2, cosine_theta)
	+(-0.02857 + 0.03689*delta)
	*gsl_sf_legendre_Plm(2, 2, cosine_theta)*cos(2.*phi);
}

int main(){

	double epsilon{1e-7};

	double w_num{0.}, w_ana{0.};

	W_pol_dir w_pol_dir_e1(
		State(0, positive), 
		{
			{Transition(electric, 2, magnetic, 4, 0.), State(2, negative)}, 
			{Transition(electric, 2, magnetic, 4, 0.),
			State(0, positive)}
		}
	);

	W_pol_dir w_pol_dir_m1(
		State(0, positive), 
		{
			{Transition(magnetic, 2, electric, 4, 0.), State(2, positive)}, 
			{Transition(magnetic, 2, electric, 4, 0.),
			State(0, positive)}
		}
	);

	for(double theta = 0.; theta < M_PI; theta += 0.5){
		for(double phi = 0.; phi < M_2_PI; phi += 0.5){
			
			w_num = w_pol_dir_e1(theta, phi);
			w_ana = w_pol_dir_0_1_0(theta, phi, electric);

			test_numerical_equality<double>(w_num, w_ana, epsilon);

			w_num = w_pol_dir_m1(theta, phi);
			w_ana = w_pol_dir_0_1_0(theta, phi, magnetic);

			test_numerical_equality<double>(w_num, w_ana, epsilon);
		}
	}

	// The criterion for correctness needs to be adapted here, since the analytical solution is given with rounded numerical factors.
	epsilon = 1e-3;

	for(double delta = -3.; delta <= 3.; delta += 0.5){
		W_pol_dir Wa_num(
			State(7, positive), 
			{
				{Transition(electric, 4, magnetic, 6, 0.), State(3, positive)}, 
				{Transition(magnetic, 2, electric, 4, delta), State(3, positive)}
			}
		);

		W_pol_dir Wb_num(
			State(3, positive), 
			{
				{Transition(magnetic, 2, electric, 4, delta), State(3, positive)}, 
				{Transition(electric, 4, magnetic, 6, 0.), State(7, positive)}
			}
		);

		for(double theta = 0.; theta < M_PI; theta += 0.5){
			for(double phi = 0.; phi < M_2_PI; phi += 0.5){
				
				// Note the different definition of phi and the additional normalization 
				// coefficient for the literature data.
				w_num = Wa_num(theta, phi);
				w_ana = Wa(theta, M_PI_2 - phi, delta)/(1.+delta*delta);

				test_numerical_equality<double>(w_num, w_ana, epsilon);

				w_num = Wb_num(theta, phi);
				w_ana = Wb(theta, M_PI_2 - phi, delta)/(1.+delta*delta);

				test_numerical_equality<double>(w_num, w_ana, epsilon);
			}
		}
	}
}
