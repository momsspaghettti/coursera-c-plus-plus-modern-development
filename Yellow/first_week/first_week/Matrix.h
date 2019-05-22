#include <iostream>
#include <vector>


using namespace std;


#pragma once
class Matrix
{
public:
	Matrix()
	{
		matrix.assign(0, vector<int>(0));
		num_rows = 0;
		num_cols = 0;
	}

	Matrix(const int& num_rows, const int& num_cols)
	{
		if (num_rows < 0 || num_cols < 0)
		{
			throw out_of_range("");
		}
		else
		{
			this->num_rows = num_rows;
			this->num_cols = num_cols;

			matrix.assign(num_rows, vector<int>(num_cols));
		}
	}

	void Reset(const int& num_rows, const int& num_cols)
	{
		if (num_rows < 0 || num_cols < 0)
		{
			throw out_of_range("");
		}
		else
		{
			this->num_rows = num_rows;
			this->num_cols = num_cols;

			matrix.assign(num_rows, vector<int>(num_cols));
		}
	}

	int At(const int& i, const int& j) const
	{

		return matrix.at(i).at(j);
	}

	int& At(const int& i, const int& j)
	{
		return matrix.at(i).at(j);
	}

	int GetNumRows() const
	{
		return this->num_rows;
	}

	int GetNumColumns() const
	{
		return this->num_cols;
	}

	bool operator==(const Matrix& other_matrix) const
	{
		if (this->GetNumRows() == other_matrix.GetNumRows() && this->GetNumColumns() == other_matrix.GetNumColumns())
		{
			for (int i = 0; i < GetNumRows(); ++i)
			{
				for (int j = 0; j < GetNumColumns(); ++j)
				{
					if (this->At(i, j) != other_matrix.At(i, j))
					{
						return false;
					}
				}
			}

			return true;
		}
		else
		{
			return false;
		}
	}

	Matrix operator+(const Matrix& other_matrix) const
	{
		if (this->GetNumRows() != other_matrix.GetNumRows() || this->GetNumColumns() != other_matrix.GetNumColumns())
		{
			throw invalid_argument("Error!");
		}

		Matrix result_matrix = Matrix(this->GetNumRows(), this->GetNumColumns());

		for (int i = 0; i < GetNumRows(); ++i)
		{
			for (int j = 0; j < GetNumColumns(); ++j)
			{
				result_matrix.At(i, j) = this->At(i, j) + other_matrix.At(i, j);
			}
		}

		return result_matrix;
	}

private:
	int num_rows;
	int num_cols;
	vector<vector<int>> matrix;
};


inline istream& operator>>(istream& stream, Matrix& matrix)
{
	int num_rows, num_cols;

	stream >> num_rows >> num_cols;

	matrix = Matrix(num_rows, num_cols);

	for (int i = 0; i < num_rows; ++i)
	{
		for (int j = 0; j < num_cols; ++j)
		{
			stream >> matrix.At(i, j);
		}
	}

	return stream;
}


inline ostream& operator<<(ostream& stream, const Matrix& matrix)
{
	stream << matrix.GetNumRows() << " " << matrix.GetNumColumns() << endl;

	for (int i = 0; i < matrix.GetNumRows(); ++i)
	{
		for (int j = 0; j < matrix.GetNumColumns(); ++j)
		{
			stream << matrix.At(i, j) << " ";
		}

		stream << endl;
	}

	return stream;
}


