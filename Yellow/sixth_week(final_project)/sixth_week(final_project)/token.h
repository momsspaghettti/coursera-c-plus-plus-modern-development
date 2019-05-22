#pragma once
#include <vector>
#include <string>


using namespace std;


enum class TokenType 
{
    DATE,
    EVENT,
    COLUMN,
    LOGICAL_OP,
    COMPARE_OP,
    PAREN_LEFT,
    PAREN_RIGHT,
};


struct Token 
{
    const string value;
    const TokenType type;
};


vector<Token> Tokenize(istream& cl);