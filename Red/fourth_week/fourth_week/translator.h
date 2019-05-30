#pragma once
#include <string_view>
#include <string>
#include <unordered_map>
#include <deque>


class Translator
{
public:
	Translator() = default;
	void Add(std::string_view source, std::string_view target);
	std::string_view TranslateForward(std::string_view source) const;
	std::string_view TranslateBackward(std::string_view target) const;

private:
	std::unordered_map<std::string_view, std::string_view> source_to_target_;
	std::unordered_map<std::string_view, std::string_view> target_to_source_;
	std::deque<std::string> storage_;
};


void TestTranslator();