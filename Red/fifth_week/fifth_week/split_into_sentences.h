#pragma once
#include <string>
#include <utility>
#include <algorithm>


struct TestToken
{
	std::string data;
	bool is_end_sentence_punctuation = false;

	bool IsEndSentencePunctuation() const
	{
		return is_end_sentence_punctuation;
	}

	bool operator==(const TestToken& other) const
	{
		return data == other.data && is_end_sentence_punctuation == other.is_end_sentence_punctuation;
	}
};

std::ostream& operator<<(std::ostream& stream, const TestToken& token);


// ќбъ€вл€ем Sentence<Token> дл€ произвольного типа Token
// синонимом vector<Token>.
// Ѕлагодар€ этому в качестве возвращаемого значени€
// функции можно указать не малопон€тный вектор векторов,
// а вектор предложений Ч vector<Sentence<Token>>.

template <typename Token>
using Sentence = std::vector<Token>;


//  ласс Token имеет метод bool IsEndSentencePunctuation() const

template <typename Token>
std::vector<Sentence<Token>> SplitIntoSentences(std::vector<Token> tokens)
{
	std::vector<Sentence<Token>> result;

	if (tokens.empty())
	{
		return result;
	}

	result.reserve(tokens.size());

	std::vector<Token> sentence;
	sentence.reserve(tokens.size());

	size_t i = 0;

    for (Token& token : tokens)
    {
        if (i != (tokens.size() - 1))
        {
            if (token.IsEndSentencePunctuation() && !tokens[i + 1].IsEndSentencePunctuation())
            {
				sentence.push_back(std::move(token));
				result.push_back(std::move(sentence));
				sentence = std::vector<Token>();
            }
            else
            {
				sentence.push_back(std::move(token));
            }
        }
        else
        {
			sentence.push_back(std::move(token));
        }

		++i;
    }
	result.push_back(std::move(sentence));

	result.shrink_to_fit();

	return result;
}


void TestSplitIntoSentences();