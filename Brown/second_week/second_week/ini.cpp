#include "ini.h"
#include <utility>


namespace Ini
{
    Section& Document::AddSection(std::string name)
    {
		return sections[std::move(name)];
    }


    const Section& Document::GetSection(const std::string& name) const
    {
		return sections.at(name);
    }


    size_t Document::SectionCount() const
    {
		return sections.size();
    }


    std::string_view ParseHeader(std::string_view line)
    {
		size_t pos = line.find_first_of('[');
		line.remove_prefix(pos + 1);

		pos = line.find_first_of(']');

		return line.substr(0, pos);
    }


    std::pair<std::string, std::string> ParseExpression(std::string_view line)
    {
		const size_t pos = line.find_first_of('=');
		const std::string_view first = line.substr(0, pos);

		line.remove_prefix(pos + 1);

		return std::make_pair(std::string(first), std::string(line));
    }


    Document Load(std::istream& input)
    {
		Document document;
		Section* section = nullptr;

        for (std::string line; std::getline(input, line);)
        {
            if (!line .empty())
            {
                if (line[0] == '[')
                {
					section = &document.AddSection(std::string(ParseHeader(line)));
                }
                else
                {
					section->insert(ParseExpression(line));
                }
            }
        }

		return document;
    }
}