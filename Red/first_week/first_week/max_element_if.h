#pragma once


void TestUniqueMax();
void TestSeveralMax();
void TestNoMax();
void TestAllMaxElemetIfTests();


template<typename ForwardIterator, typename UnaryPredicate>
ForwardIterator max_element_if(ForwardIterator first, ForwardIterator last, UnaryPredicate predicate);


template <typename ForwardIterator, typename UnaryPredicate>
ForwardIterator max_element_if(ForwardIterator first, ForwardIterator last, UnaryPredicate predicate)
{
	auto max = last;
	bool flag = false;

    for (; first != last; ++first)
    {
        if (predicate(*first) && !flag)
        {
			max = first;
			flag = true;
        }

        if (flag)
        {
            if (predicate(*first) && *first > *max)
            {
				max = first;
            }
        }
    }

	return max;
}