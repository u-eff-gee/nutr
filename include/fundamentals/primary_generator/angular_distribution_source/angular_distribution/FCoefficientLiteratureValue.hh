#include <vector>

using std::vector;

/**
 * \brief Struct to store F-coefficient parameters and their values from literature data.
 */
struct FCoefficientLiteratureValue{
	/**
	 * \brief Constructor for FCoefficientLiteratureValue
	 *
	 * The order of the parameters corresponds to the order in the tables of 
	 * Ref. \cite FerentzRosenzweig1955.
	 */
	FCoefficientLiteratureValue(int t_j, int t_jp, int t_L, int t_Lp, int t_nu, double val):
		two_j(t_j),
		two_jp(t_jp),
		two_L(t_L),
		two_Lp(t_Lp),
		two_nu(t_nu),
		value(val)
	{};
	~FCoefficientLiteratureValue() = default;

	int two_j, two_jp, two_L, two_Lp, two_nu;
	double value;
};
