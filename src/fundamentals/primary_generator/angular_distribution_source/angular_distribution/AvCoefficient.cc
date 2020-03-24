#include "AvCoefficient.hh"

AvCoefficient::AvCoefficient():
f_coef(FCoefficient())
{}

double AvCoefficient::operator()(const int two_nu, const int two_L, const int two_Lp, const int two_jn, const int two_j, const double delta) const {
	
	return 	f_coef(two_nu, two_L, two_L, two_jn, two_j)
		+ 2.*delta*f_coef(two_nu, two_L, two_Lp, two_jn, two_j)
		+ delta*delta*f_coef(two_nu, two_Lp, two_Lp, two_jn, two_j);
}
