#pragma once
#include <vector>


template<typename T>
class Table
{
public:
	Table(const size_t& n, const size_t& m) : n_(n), m_(m)
	{
		matrix_ = std::vector<std::vector<T>>(n_, std::vector<T>(m_));
	}

    void Resize(const size_t& n, const size_t& m)
	{
		n_ = n;
	    m_ = m;

		matrix_.resize(n_);

        for (auto& line : matrix_)
        {
			line.resize(m_);
        }
	}

    std::pair<size_t, size_t> Size() const
	{
		return std::make_pair(n_, m_);
	}

    const std::vector<T>& operator[](const size_t& pos) const
	{
		return matrix_[pos];
	}

    std::vector<T>& operator[](const size_t pos)
	{
		return matrix_[pos];
	}

private:
	size_t n_;
	size_t m_;

	std::vector<std::vector<T>> matrix_;
};


void TableTests();


void TestTable();