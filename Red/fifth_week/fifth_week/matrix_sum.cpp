#include "UnitTestsFramework.h"
#include "matrix_sum.h"
#include <numeric>
#include "paginator.h"
#include <vector>
#include <iterator>
#include "Profiler.h"
#include <utility>


template<typename Container>
int64_t MatrixSum(const Container& matrix)
{
	int64_t result = 0;

    for (const auto& line : matrix)
    {
		result += std::accumulate(line.begin(), line.end(), 0);
    }

	return result;
}


int64_t CalculateMatrixSum(const std::vector<std::vector<int>>& matrix)
{
	int64_t result = 0;

	std::vector<std::future<int64_t>> futures;
	futures.reserve(4);

	for (auto page : Paginate(matrix, 2000))
	{

	    futures.push_back(std::async([=]
		{
			return MatrixSum(page);
		}));
	}

	for (auto& future_ : futures)
	{
		result += future_.get();
	}

	return result;
}


void TestCalculateMatrixSum() 
{
    const std::vector<std::vector<int>> matrix = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16}
    };
  
    ASSERT_EQUAL(CalculateMatrixSum(matrix), 136);
}


void TestPerformance()
{
	std::vector<std::vector<int>> matrix;
	matrix.reserve(9000);

    for (int i = 0; i < 9000; ++i)
    {
		std::vector<int> line;
		line.reserve(9000);

        for (int j = 0; j < 9000; ++j)
        {
			line.push_back(j + i);
        }

		matrix.push_back(std::move(line));
    }

	int64_t expected = 0;
	int64_t result = 0;

	{
		TIME_IT("OneThread");
		for (const auto& line : matrix)
		{
			expected += std::accumulate(line.begin(), line.end(), 0);
		}
	}

	{
		TIME_IT("MultiThread");
		result = CalculateMatrixSum(matrix);
	}

	ASSERT_EQUAL(result, expected);
}


void TestMatrixSum() 
{
    TestRunner tr;
    RUN_TEST(tr, TestCalculateMatrixSum);
	//RUN_TEST(tr, TestPerformance);
}