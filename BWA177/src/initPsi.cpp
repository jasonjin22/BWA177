#include "genPsi/initPsi.hpp"

std::vector<int> getSA(std::string segment) {
	std::vector<int> result;
	int segment_len = segment.length();
	std::list< std::tuple<std::string, int> > sort_list;
	for (int i = 0; i < segment_len; ++i) {
		sort_list.push_back(std::make_tuple(segment, i));
		segment = segment.substr(1, segment_len - 1) + segment[0];
	}
	sort_list.sort();
	for (auto s : sort_list) {
		result.push_back(std::get<1>(s));
	}
	return result;
}

std::vector<int> get_inv_SA(const std::vector<int> sa) {
	int sa_len = sa.size();
	std::vector<int> result(sa_len);
	// the definition of the inverse suffix array
	for (int i = 0; i < sa_len; ++i) {
		result[sa[i]] = i;
	}
	return result;
}

std::vector<int> construct_Psi(const std::vector<int> sa, const std::vector<int> inv_sa) {
	int sa_len = sa.size();
	std::vector<int> result(sa_len);
	// the first element in Psi is the first element in inv_sa
	result[0] = inv_sa[0];
	for (int i = 1; i < sa_len; ++i) {
		// definition of the Psi array
		result[i] = inv_sa[sa[i] + 1];
	}
	return result;
}

std::vector<int> brute_force_init_Psi(const std::string segment) {
	std::vector<int> sa = getSA(segment);
	std::vector<int> inv_sa = get_inv_SA(sa);
	return construct_Psi(sa, inv_sa);
}