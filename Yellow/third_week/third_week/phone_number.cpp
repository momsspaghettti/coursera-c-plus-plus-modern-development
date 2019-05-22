#include "pch.h"
#include "phone_number.h"
#include <exception>
#include <stdexcept>


PhoneNumber::PhoneNumber(const string& international_number)
{
    if (!international_number.empty())
    {
        if (international_number[0] == '+')
        {
			size_t i = 1;

            try
            {
                while (i < international_number.size() && international_number[i] != '-')
                {
					country_code_ += international_number[i];
					++i;
                }

                if (country_code_.empty())
                {
					throw invalid_argument("empty country code!");
                }
            }
            catch (std::exception&)
            {
				throw invalid_argument("no country code!");
            }

			++i;

			try
			{
				while (i < international_number.size() && international_number[i] != '-')
				{
					city_code_ += international_number[i];
					++i;
				}

				if (city_code_.empty())
				{
					throw invalid_argument("empty city code!");
				}
			}
			catch (std::exception&)
			{
				throw invalid_argument("no city code!");
			}

			++i;

			for (size_t j = i; j < international_number.size(); ++j)
			{
				local_number_ += international_number[j];
			}

            if (local_number_.empty())
            {
				throw invalid_argument("empty local number!");
            }
        }
        else
        {
			throw invalid_argument("number without '+'!");
        }
    }
    else
    {
		throw invalid_argument("empty number!");
    }
}


string PhoneNumber::GetCountryCode() const
{
	return country_code_;
}


string PhoneNumber::GetCityCode() const
{
	return city_code_;
}


string PhoneNumber::GetLocalNumber() const
{
	return local_number_;
}


string PhoneNumber::GetInternationalNumber() const
{
	return "+" + country_code_ + "-" + city_code_ + "-" + local_number_;
}


