#include "json.h"
#include <cstdint>
#include <utility>
#include <sstream>


namespace Json
{

	Document::Document(Node root) : root(move(root)) {}

	const Node& Document::GetRoot() const
    {
		return root;
	}

	Node LoadNode(std::istream& input);

	Node LoadArray(std::istream& input) 
    {
        std::vector<Node> result;

		for (char c; input >> c && c != ']'; ) 
		{
			if (c != ',') 
			{
				input.putback(c);
			}
			result.push_back(LoadNode(input));
		}

		return Node(move(result));
	}

	Node LoadNumericalData(std::istream& input)
    {
        if (input.peek() == '-')
        {
			double result = 0;
			input.ignore(1);

			while (std::isdigit(input.peek()))
			{
				result *= 10;

				result += input.get() - '0';
			}

			if (input.peek() == '.')
			{
				uint64_t degree = 10;
				input.ignore(1);

				while (std::isdigit(input.peek()))
				{
					result += (input.get() - '0') * 1.0 / degree;
					degree *= 10;
				}
			}

			return Node(-result);
        }

		unsigned int_result = 0;
		double double_result = 0;

		while (std::isdigit(input.peek()))
		{
			int_result *= 10;
			double_result *= 10;

			const auto tmp = input.get() - '0';
			int_result += tmp;
			double_result += tmp;
		}

        if (input.peek() == '.')
        {
			uint64_t degree = 10;
			input.ignore(1);

            while (std::isdigit(input.peek()))
            {
				double_result += (input.get() - '0') * 1.0 / degree;
				degree *= 10;
            }

			return Node(double_result);
        }

		return Node(int_result);
	}

	Node LoadString(std::istream& input) 
    {
        std::string line;
		std::getline(input, line, '"');
		return Node(std::move(line));
	}

	Node LoadDict(std::istream& input)
    {
        std::map<std::string, Node> result;

		for (char c; input >> c && c != '}'; )
		{
			if (c == ',')
			{
				input >> c;
			}

            if (c == '"')
            {
				auto key = LoadString(input).AsString();
				input >> c;
				result.emplace(std::move(key), LoadNode(input));
            }
		}

		return Node(std::move(result));
	}

    Node LoadBool(std::istream& input)
	{
        if (input.peek() == 't')
        {
			input.ignore(4);
			return Node(true);
        }
        else
        {
			input.ignore(5);
			return Node(false);
        }
	}

	Node LoadNode(std::istream& input)
    {
		char c;
		input >> c;

		if (c == '[') 
		{
			return LoadArray(input);
		} 
	    else if (c == '{') 
		{
			return LoadDict(input);
		} 
	    else if (c == '"')
		{
			return LoadString(input);
		}
		else if (c == 't' || c == 'f')
		{
			input.putback(c);
			return LoadBool(input);
		}
	    else if (c == '-' || std::isdigit(c))
		{
			input.putback(c);
			return LoadNumericalData(input);
		}

		return LoadNode(input);
	}

	Document Load(std::istream& input)
    {
		return Document{ LoadNode(input) };
	}


    std::string JsonArray::ToString() const
    {
		std::string result = std::string("[ ");

		bool first_item = true;
        for (const auto& item : array_)
        {
            if (first_item)
            {
				result += item->ToString();
				first_item = false;
            }
            else
            {
				result += std::string(", ") + item->ToString();
            }
        }

		result += std::string(" ]");
		return result;
    }

    std::string JsonMap::ToString() const
    {
		std::string result = std::string("{ ");

		bool first_item = true;
        for (const auto& p : map_)
        {
			if (first_item)
			{
				result += std::string("\"") + p.first + 
					std::string("\"") + std::string(": ") + p.second->ToString();
				first_item = false;
			} else
			{
				result += std::string(", ") + std::string("\"") + 
					p.first + std::string("\"") + std::string(": ") + p.second->ToString();
			}
        }

		result += std::string(" }");
		return result;
    }

    std::string JsonInt::ToString() const
    {
		return std::to_string(item_);
    }

    std::string JsonDouble::ToString() const
    {
		std::stringstream tmp;
		tmp.precision(6);
		tmp << item_;

		return tmp.str();
    }

    std::string JsonString::ToString() const
    {
		return std::string("\"") + item_ + std::string("\"");
    }

    std::string JsonBool::ToString() const
    {
		if (flag_)
			return std::string("true");

		return std::string("false");
    }
}