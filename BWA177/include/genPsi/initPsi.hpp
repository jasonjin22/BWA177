#include <iostream>
#include <vector>
#include <list>
#include <tuple>

/**
 * \brief directly get the suffix array of a string
 * \details called in brute_force_init_Psi(), directly modify the input string and sort it,
 * only available with short input because the space cost is O(n^2)
 * 
 * \param segment the string we want the suffix array
 * \return the suffix array of the string
 */
std::vector<int> getSA(const std::string segment);

/**
 * \brief get the inverse suffix array of a suffix array
 * \details inverse the given suffix array,  only call in brute_force_init_Psi
 * 
 * \param sa the input suffix array
 * \return the inverse suffix array
 */
std::vector<int> get_inv_SA(const std::vector<int> sa);

/**
 * \brief construct the Psi array with computed suffix array and inverse suffix array
 * \details construct the Psi array which can be used to construct the BWT, only call in 
 * brute_force_init_Psi
 * 
 * \param sa the suffix array
 * \param inv_sa the inversed suffix array
 * 
 * \return the Psi array which can be used to construct the BWT
 */
std::vector<int> construct_Psi(const std::vector<int> sa, const std::vector<int> inv_sa);

/**
 * \brief directly compute the Psi array for a short input string
 * \details to incrementally compute the Psi array of a long string, we should first directly compute the initial short segment
 * 
 * \param segment the initial short segment
 * \return the initial Psi array, which can be used to compute the whole Psi array step by step
 */
std::vector<int> brute_force_init_Psi(const std::string segment);