#pragma once
#include <vector>
#include <iterator>


template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end);


template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) 
{
    if (range_end - range_begin < 2)
    {
        return;
    }

	std::vector<typename RandomIt::value_type> tmp_vec(std::make_move_iterator(range_begin), 
		std::make_move_iterator(range_end));

	const int left_border = (tmp_vec.end() - tmp_vec.begin()) / 3;
	const int right_border = left_border * 2;

	MergeSort(std::move(tmp_vec.begin()), std::move(tmp_vec.begin() + left_border));
	MergeSort(std::move(tmp_vec.begin() + left_border), std::move(tmp_vec.begin() + right_border));
	MergeSort(std::move(tmp_vec.begin() + right_border), std::move(tmp_vec.end()));

	std::vector<typename RandomIt::value_type> also_tmp_vec;

	std::merge(std::make_move_iterator(tmp_vec.begin()), 
		std::make_move_iterator(tmp_vec.begin() + left_border), 
		std::make_move_iterator(tmp_vec.begin() + left_border), 
		std::make_move_iterator(tmp_vec.begin() + right_border), 
		std::back_inserter(also_tmp_vec));

	std::merge(std::make_move_iterator(also_tmp_vec.begin()), 
		std::make_move_iterator(also_tmp_vec.end()), 
		std::make_move_iterator(tmp_vec.begin() + right_border), 
		std::make_move_iterator(tmp_vec.end()), 
		range_begin);
}


void TestMergeSort();