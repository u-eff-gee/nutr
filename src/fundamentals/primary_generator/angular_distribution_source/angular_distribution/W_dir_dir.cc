#include "W_dir_dir.hh"

W_dir_dir::W_dir_dir(const State ini_state, const Transition ini_to_int, const State int_state, const Transition int_to_fin, const State fin_state):
initial_state(ini_state), initial_to_intermediate(ini_to_int),
intermediate_state(int_state), intermediate_to_final(int_to_fin),
final_state(fin_state)
{
	//two_nu_max = get_two_nu_max();	
}

int W_dir_dir::get_two_nu_max(const int two_L_n, const int two_L_p) const {}
