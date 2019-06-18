#include "xml.h"
#include <string_view>
#include <iostream>


namespace Xml
{
    std::pair<std::string_view, std::string_view> Split(std::string_view line, char by)
    {
        const size_t pos = line.find(by);
        std::string_view left = line.substr(0, pos);

        if (pos < line.size() && pos + 1 < line.size())
        {
            return { left, line.substr(pos + 1) };
        }
        else
        {
            return { left, std::string_view() };
        }
    }


    std::string_view Lstrip(std::string_view line)
    {
        while (!line.empty() && isspace(line[0]))
        {
            line.remove_prefix(1);
        }

        return line;
    }


    std::string_view Unquote(std::string_view value)
    {
        if (!value.empty() && value.front() == '"')
        {
            value.remove_prefix(1);
        }
        if (!value.empty() && value.back() == '"')
        {
            value.remove_suffix(1);
        }
        return value;
    }


    Node LoadNode(std::istream& input)
    {
        std::string root_name;
        std::getline(input, root_name);

        Node root(root_name.substr(1, root_name.size() - 2), {});
	
        for (std::string line; getline(input, line) && Lstrip(line)[1] != '/'; )
        {
            auto [node_name, attrs] = Split(Lstrip(line), ' ');
            attrs = Split(attrs, '>').first;
            std::unordered_map<std::string, std::string> node_attrs;
		
            while (!attrs.empty())
            {
                auto [head, tail] = Split(attrs, ' ');
                auto [name, value] = Split(head, '=');
			
                if (!name.empty() && !value.empty())
                {
                    node_attrs[std::string(Unquote(name))] = std::string(Unquote(value));
                }
                attrs = tail;
            }

            root.AddChild(Node(std::string(node_name.substr(1)), 
                               std::move(node_attrs)));
        }

        return root;
    }


    Document Load(std::istream& input)
    {
        return Document{ LoadNode(input) };
    }


    Node::Node(std::string name, 
               std::unordered_map<std::string, std::string> attrs) : name(move(name)), attrs(move(attrs)){}


    const std::vector<Node>& Node::Children() const
    {
        return children;
    }


    Document::Document(Node root) : root(std::move(root)){}


    const Node& Document::GetRoot() const
    {
        return root;
    }


    void Node::AddChild(Node node)
    {
        children.push_back(std::move(node));
    }


    std::string_view Node::Name() const
    {
        return name;
    }
}