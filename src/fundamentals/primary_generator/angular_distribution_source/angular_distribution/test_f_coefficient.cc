#include "FCoefficient.hh"
#include "FCoefficientLiteratureValue.hh"
#include "TestUtilities.hh"

vector<FCoefficientLiteratureValue> f_coefficient_values {
	FCoefficientLiteratureValue(2, 0, 2, 2, 4,  0.70710680),
	FCoefficientLiteratureValue(2, 2, 0, 4, 4,  0.00000000),
	FCoefficientLiteratureValue(2, 2, 2, 2, 4, -0.35355340),
	FCoefficientLiteratureValue(2, 2, 2, 4, 4, -1.06066017),
	FCoefficientLiteratureValue(2, 2, 4, 4, 4, -0.35355340),
	FCoefficientLiteratureValue(2, 4, 2, 2, 4,  0.07071068),
	FCoefficientLiteratureValue(2, 4, 2, 4, 4,  0.47434165),
	FCoefficientLiteratureValue(2, 4, 2, 6, 4,  0.84852814),
	FCoefficientLiteratureValue(2, 4, 4, 4, 4,  0.35355340),
	FCoefficientLiteratureValue(2, 4, 4, 6, 4, -0.63245554),
	FCoefficientLiteratureValue(2, 4, 6, 6, 4, -0.42426407),
	FCoefficientLiteratureValue(2, 6, 4, 4, 4, -0.10101526),
	FCoefficientLiteratureValue(2, 6, 4, 6, 4,  0.37796448),
	FCoefficientLiteratureValue(2, 6, 4, 8, 4,  0.95831485),
	FCoefficientLiteratureValue(2, 6, 6, 6, 4,  0.53033009),
	FCoefficientLiteratureValue(2, 6, 6, 8, 4, -0.44821072),
	FCoefficientLiteratureValue(2, 6, 8, 8, 4, -0.42931483),
	FCoefficientLiteratureValue(2, 8, 6, 6, 4, -0.17677670),
	FCoefficientLiteratureValue(2, 8, 6, 8, 4,  0.30618621),
	FCoefficientLiteratureValue(2, 8, 6,10, 4,  0.99999997),
	FCoefficientLiteratureValue(2, 8, 8, 8, 4,  0.60104077),
	FCoefficientLiteratureValue(2, 8, 8,10, 4, -0.34641016),
	FCoefficientLiteratureValue(2, 8,10,10, 4, -0.42426407),
	FCoefficientLiteratureValue(2,10, 8, 8, 4, -0.21856028),
	FCoefficientLiteratureValue(2,10, 8,10, 4,  0.25584086),
};

int main(){

	FCoefficient f_coef;
	const double epsilon = 1e-7;

	double f_coef_num{0.};

	for(auto f : f_coefficient_values){
		f_coef_num = f_coef(f.two_nu, f.two_L, f.two_Lp, f.two_jp, f.two_j);
		// Test whether the numerical value is correct within the given digits of the 
		// literature data.
		test_numerical_equality<double>(f_coef_num, f.value, epsilon);
		// Check whether the prediction that a given F coefficient vanishes is correct.
		assert(f_coef.is_nonzero(f.two_nu, f.two_L, f.two_Lp, f.two_jp, f.two_j) == (f_coef_num != 0.));
	}

}
