#pragma once
#include <string>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <string_view>


class Domain
{
public:
	explicit Domain(const std::string& domains);

	[[nodiscard]] bool IsSubDomain(const Domain& sub_domain) const;

	std::vector<std::string> domain_tokens;

private:
	std::vector<std::string> split(const std::string& domains);

	std::vector<std::string> reverse(const std::vector<std::string>& tokens);
};


class DomainsFilter
{
public:
	void AddForbiddenDomain(const std::string& domain);

	[[nodiscard]] bool CheckDomain(const std::string& domain) const;

private:
	std::unordered_map<std::string, std::vector<Domain>> domains_;

	static std::string get_domain_head(std::string_view domain);
};


void PerformDomainsQueries(std::istream& input = std::cin, std::ostream& output = std::cout);


void TestForbiddenDomains();