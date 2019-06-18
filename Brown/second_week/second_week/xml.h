#pragma once
#include <istream>
#include <sstream>
#include <vector>
#include <string>
#include <string_view>
#include <unordered_map>


namespace Xml
{
    class Node
    {
    public:
        Node(std::string name, std::unordered_map<std::string, std::string> attrs);

        const std::vector<Node>& Children() const;
	
        void AddChild(Node node);
    
        std::string_view Name() const;

        template <typename T>
        T AttributeValue(const std::string& name) const;

    private:
        std::string name;
        std::vector<Node> children;
        std::unordered_map<std::string, std::string> attrs;
    };


    class Document 
    {
    public:
        explicit Document(Node root);

        const Node& GetRoot() const;

    private:
        Node root;
    };


    Document Load(std::istream& input);


    template <typename T>
    T Node::AttributeValue(const std::string& name) const 
    {
        std::istringstream attr_input(attrs.at(name));
        T result;
        attr_input >> result;
	
        return result;
    }
}