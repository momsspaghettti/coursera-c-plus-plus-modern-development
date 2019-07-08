#pragma once
#include <istream>
#include <string>
#include <variant>
#include <vector>
#include <memory>
#include <utility>
#include <map>
#include <cstdint>


namespace Json
{
	class Node : std::variant<std::vector<Node>,
    std::map<std::string, Node>, unsigned, double, std::string, bool>
    {
	public:
		using variant::variant;

		[[nodiscard]] const auto& AsArray() const 
	    {
			return std::get<std::vector<Node>>(*this);
		}

		[[nodiscard]] const auto& AsMap() const 
	    {
			return std::get<std::map<std::string, Node>>(*this);
		}

		[[nodiscard]] const auto& AsInt() const
	    {
			return std::get<unsigned>(*this);
		}

		[[nodiscard]] const double AsDouble() const
		{
            try
            {
				return std::get<double>(*this);
            }
            catch (std::bad_variant_access&)
            {
				return static_cast<double>(std::get<unsigned>(*this));
            }
		}

		[[nodiscard]] const auto& AsString() const
	    {
			return std::get<std::string>(*this);
		}

		[[nodiscard]] const auto& AsBool() const
		{
			return std::get<bool>(*this);
		}
	};

	class Document 
    {
	public:
		explicit Document(Node root);

		[[nodiscard]] const Node& GetRoot() const;

	private:
		Node root;
	};


	Document Load(std::istream& input);


    class JsonWriter
    {
    public:
		[[nodiscard]] virtual std::string ToString() const = 0;

		virtual ~JsonWriter() = default;
    };

    class JsonArray : public JsonWriter
    {
    public:
        explicit JsonArray(const std::vector<std::shared_ptr<JsonWriter>>& vec)
        {
			array_ = vec;
        }

		[[nodiscard]] std::string ToString() const override;

    private:
		std::vector<std::shared_ptr<JsonWriter>> array_;
    };

    class JsonMap : public JsonWriter
    {
    public:
		explicit JsonMap(const std::map<std::string,
			std::shared_ptr<JsonWriter>>& m)
        {
			map_ = m;
        }

		[[nodiscard]] std::string ToString() const override;

    private:
		std::map<std::string, 
        std::shared_ptr<JsonWriter>> map_;
    };

    class JsonInt : public JsonWriter
    {
    public:
		explicit JsonInt(uint64_t num) : item_(num) {};

		[[nodiscard]] std::string ToString() const override;

    private:
		uint64_t item_;
    };

    class JsonDouble : public JsonWriter
    {
    public:
		explicit JsonDouble(double num) : item_(num) {}

		[[nodiscard]] std::string ToString() const override;

    private:
		double item_;
    };

    class JsonString : public JsonWriter
    {
	public:
		explicit JsonString(std::string s) : item_(std::move(s)) {}

		[[nodiscard]] std::string ToString() const override;

	private:
		std::string item_;
    };

    class JsonBool : public JsonWriter
    {
    public:
		explicit JsonBool(bool f) : flag_(f) {};

		[[nodiscard]] std::string ToString() const override;

    private:
		bool flag_;
    };
}
