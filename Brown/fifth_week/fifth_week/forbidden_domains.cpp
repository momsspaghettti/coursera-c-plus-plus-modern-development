#include "forbidden_domains.h"
#include "UnitTestsFramework.h"
#include <sstream>


std::vector<std::string> Domain::split(const std::string& domains)
{
	std::vector<std::string> result;
	
    std::string token;
	for (const auto& ch : domains)
	{
		if (ch == '.')
		{
			result.push_back(token);
			token = "";
		} else
		{
			token += ch;
		}
	}

	result.push_back(token);

	return result;
}


std::vector<std::string> Domain::reverse(const std::vector<std::string>& tokens)
{
	std::vector<std::string> result;
	result.reserve(tokens.size());

    for (auto token = tokens.rbegin(); token != tokens.rend(); ++token)
    {
		result.push_back(*token);
    }

	return result;
}


bool Domain::IsSubDomain(const Domain& sub_domain) const
{
    if (domain_tokens.size() < sub_domain.domain_tokens.size())
        return false;

	size_t i = 0;
    for (const auto& token : sub_domain.domain_tokens)
    {
		if (token != domain_tokens.at(i))
			return false;

		++i;
    }

    return true;
}


Domain::Domain(const std::string& domains)
{
	domain_tokens = reverse(split(domains));
}


std::string DomainsFilter::get_domain_head(std::string_view domain)
{
	const auto pos = domain.find_last_of('.');

	if (pos == domain.npos)
		return std::string(domain);

	return std::string(domain.substr(pos + 1, domain.size() - pos));
}


void DomainsFilter::AddForbiddenDomain(const std::string& domain)
{
	domains_[get_domain_head(domain)].push_back(Domain(domain));
}


bool DomainsFilter::CheckDomain(const std::string& domain) const
{
    if (domains_.count(get_domain_head(domain)) == 0)
        return false;

	const Domain checking_domain = Domain(domain);

	const auto& domains = domains_.at(get_domain_head(domain));

	for (const Domain& domain_ : domains)
		if (checking_domain.IsSubDomain(domain_))
			return true;

    return false;
}


void PerformDomainsQueries(std::istream& input, std::ostream& output)
{
	DomainsFilter filter;

	int n;
	input >> n;
	std::string domain;
    for (int i = 0; i < n; ++i)
    {
		input >> domain;
		filter.AddForbiddenDomain(domain);
    }

	int m;
	input >> m;

    for (int i = 0; i < m; ++i)
    {
		input >> domain;

        if (filter.CheckDomain(domain))
        {
			output << "Bad";
        }
        else
        {
			output << "Good";
        }

		output << '\n';
    }
}


void TestDomainsFilter()
{
	DomainsFilter filter;

	filter.AddForbiddenDomain("ya.ru");
	filter.AddForbiddenDomain("maps.me");
	filter.AddForbiddenDomain("m.ya.ru");
	filter.AddForbiddenDomain("com");

	ASSERT(filter.CheckDomain("ya.ru"));
	ASSERT(filter.CheckDomain("ya.com"));
	ASSERT(filter.CheckDomain("m.maps.me"));
	ASSERT(filter.CheckDomain("moscow.m.ya.ru"));
	ASSERT(filter.CheckDomain("maps.com"));
	ASSERT(!filter.CheckDomain("maps.ru"));
	ASSERT(!filter.CheckDomain("ya.ya"));
	ASSERT(filter.CheckDomain("com"));
	ASSERT(!filter.CheckDomain("mmaps.me"));
    ASSERT(!filter.CheckDomain("mamya.ru"))
}


void TestPerformDomainsQueries()
{
	std::stringstream input;
	input << "4\n" << "ya.ru\n" << "maps.me\n" << "m.ya.ru\n" << "com\n" << "7\n" << "ya.ru\n" << "ya.com\n";
	input << "m.maps.me\n" << "moscow.m.ya.ru\n" << "maps.com\n" << "maps.ru\n" << "ya.ya\n";

	std::stringstream output;
	PerformDomainsQueries(input, output);
	std::string expected = 
		"Bad\n"
        "Bad\n"
        "Bad\n"
        "Bad\n"
		"Bad\n"
		"Good\n"
		"Good\n";

	ASSERT_EQUAL(output.str(), expected);
}


void TestForbiddenDomains()
{
	TestRunner tr;

	RUN_TEST(tr, TestDomainsFilter);
	RUN_TEST(tr, TestPerformDomainsQueries);
}