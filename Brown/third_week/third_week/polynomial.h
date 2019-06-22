#pragma once
#include <vector>


template<typename T>
class IndexProxy
{
public:
	IndexProxy(std::vector<T>& v, size_t pos) : coeffs_(v), pos_(pos) {}

	void operator=(T& val) const // getter
	{
		if (pos_ < coeffs_.size())
			val = coeffs_[pos_];

		val = 0;
	}

	void operator=(const T& val) // setter
	{
        if (pos_ >= coeffs_.size())
        {
            if (val != 0)
            {
				if (pos_ >= coeffs_.size())
					coeffs_.resize(pos_ + 1);

				coeffs_[pos_] = val;
            }
        }
        else
        {
			coeffs_[pos_] = val;

			shrink();
        }
	}


	std::vector<T>& coeffs_;
	size_t pos_;

private:
	void shrink()
	{
		while (!coeffs_.empty() && coeffs_.back() == 0)
		{
			coeffs_.pop_back();
		}
	}
};


template<typename T>
bool operator!=(const IndexProxy<T>& index, const T& other)
{
	if (index.pos_ >= index.coeffs_.size())
		return 0 != other;

	return index.coeffs_[index.pos_] != other;
}


template<typename T>
bool operator==(const IndexProxy<T>& index, const T& other)
{
	if (index.pos_ >= index.coeffs_.size())
		return 0 == other;

	return index.coeffs_[index.pos_] == other;
}


template<typename T>
std::ostream& operator<<(std::ostream& out, const IndexProxy<T>& index)
{
	if (index.pos_ >= index.coeffs_.size())
	{
		out << 0;
	}
	else
	{
		out << index.coeffs_.at(index.pos_);
	}

	return out;
}


template<typename T>
void PrintCoeff(std::ostream& out, int i, const T& coef, bool printed);


template<typename T>
class Polynomial
{
private:
	std::vector<T> coeffs_ = { 0 };

	void Shrink()
    {
		while (!coeffs_.empty() && coeffs_.back() == 0) 
		{
			coeffs_.pop_back();
		}
	}

public:
	Polynomial() = default;

	Polynomial(std::vector<T> coeffs) : coeffs_(std::move(coeffs)) 
    {
		Shrink();
	}


	template<typename Iterator>
	Polynomial(Iterator first, Iterator last) : coeffs_(first, last)
    {
		Shrink();
	}


	bool operator ==(const Polynomial& other) const 
    {
		return coeffs_ == other.coeffs_;
	}


	bool operator !=(const Polynomial& other) const 
    {
		return !operator==(other);
	}


	int Degree() const 
    {
		return coeffs_.size() - 1;
	}


	Polynomial& operator +=(const Polynomial& r)
    {
		if (r.coeffs_.size() > coeffs_.size()) 
		{
			coeffs_.resize(r.coeffs_.size());
		}
		
	    for (size_t i = 0; i != r.coeffs_.size(); ++i) 
		{
			coeffs_[i] += r.coeffs_[i];
		}
		Shrink();
		
	    return *this;
	}

	Polynomial& operator -=(const Polynomial& r) 
    {
		if (r.coeffs_.size() > coeffs_.size()) 
		{
			coeffs_.resize(r.coeffs_.size());
		}

		for (size_t i = 0; i != r.coeffs_.size(); ++i) 
		{
			coeffs_[i] -= r.coeffs_[i];
		}
		Shrink();
		
	    return *this;
	}


	T operator[](size_t pos) const
	{
		if (pos < coeffs_.size())
			return coeffs_[pos];

		return 0;
	}

	
    IndexProxy<T> operator[](size_t pos)
	{
		return IndexProxy<T>(coeffs_, pos);
	}


	T operator ()(const T& x) const 
    {
		T res = 0;
		for (auto it = coeffs_.rbegin(); it != coeffs_.rend(); ++it) 
		{
			res *= x;
			res += *it;
		}
		return res;
	}

	using const_iterator = typename std::vector<T>::const_iterator;

	const_iterator begin() const
    {
		return coeffs_.cbegin();
	}

	const_iterator end() const 
    {
		return coeffs_.cend();
	}
};


template <typename T>
std::ostream& operator <<(std::ostream& out, const Polynomial<T>& p) 
{
	bool printed = false;
	for (int i = p.Degree(); i >= 0; --i) 
	{
		if (p[i] != 0) 
		{
			PrintCoeff(out, i, p[i], printed);
			printed = true;
		}
	}
	return out;
}


template <typename T>
Polynomial<T> operator +(Polynomial<T> lhs, const Polynomial<T>& rhs)
{
	lhs += rhs;
	return lhs;
}


template <typename T>
Polynomial<T> operator -(Polynomial<T> lhs, const Polynomial<T>& rhs)
{
	lhs -= rhs;
	return lhs;
}


template<typename T>
void PrintCoeff(std::ostream& out, int i, const T& coef, bool printed)
{
	bool coeffPrinted = false;
	if (coef == 1 && i > 0)
	{
		out << (printed ? "+" : "");
	}
	else if (coef == -1 && i > 0)
	{
		out << "-";
	}
	else if (coef >= 0 && printed)
	{
		out << "+" << coef;
		coeffPrinted = true;
	}
	else
	{
		out << coef;
		coeffPrinted = true;
	}

	if (i > 0)
	{
		out << (coeffPrinted ? "*" : "") << "x";
	}

	if (i > 1)
	{
		out << "^" << i;
	}
}


void TestPolynomial();