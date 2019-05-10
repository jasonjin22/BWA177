#ifndef SEGMENT_HPP
#define SEGMENT_HPP

/**
 * \brief denote the start index and end index of a string
 * \details used together with the fa vector, instead of passing the whole string into a function,
 * pass this class into a function saves a lot of space
 * 
 * \param start_index start index of the sequence (include the start index)
 * \param end_index end index of the sequence (include the end index)
 * 
 * \return nothing
 */
class segment {
public:
	segment(int start_index, int end_index);
	~segment();
	int get_start_index();
	int get_end_index();
	int get_length();
	void increment_head(segment seg);

private:
	// both the start and end index are included in the representing string
	int _start_index;
	int _end_index;
};

#endif
