#include "genPsi/initPsi.hpp"
#include "compressfa/extractfa.hpp"

std::vector<int> getSA(std::string seg) {
	std::vector<int> result;
	int seg_len = seg.length();
	std::list< std::tuple<std::string, int> > sort_list;
	for (int i = 0; i < seg_len; ++i) {
		sort_list.push_back(std::make_tuple(seg, i));
		seg = seg.substr(1, seg_len - 1) + seg[0];
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

std::vector<int> brute_force_init_Psi(std::vector<uint8_t> * const fa, segment seg) {
	std::string seg_str = "";
	int start_index = seg.get_start_index();
	int end_index = seg.get_end_index();
	for (int i = start_index; i < end_index + 1; ++i) {
		seg_str += extractfa(fa, i);
	}
	std::vector<int> sa = getSA(seg_str);
	std::vector<int> inv_sa = get_inv_SA(sa);
	return construct_Psi(sa, inv_sa);
}