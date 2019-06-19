#pragma once
#include <map>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>


struct HttpRequest 
{
	std::string method, path, body;
	std::map<std::string, std::string> get_params;
};


struct LastCommentInfo
{
	size_t user_id, consecutive_count;
};


struct HttpHeader
{
    std::string name, value;
};


struct ParsedResponse
{
	int code;
	std::vector<HttpHeader> headers;
    std::string content;
};


enum class HttpCode
{
	Ok = 200,
	NotFound = 404,
	Found = 302,
};


class HttpResponse
{
public:
	explicit HttpResponse(HttpCode code) : code_(code)
	{
		comment_ = comment_map_.at(code_);
	};

	HttpResponse& AddHeader(std::string name, std::string value)
	{
		headers_.insert({ name, value });
		return *this;
	}
	
    HttpResponse& SetContent(std::string a_content)
	{
		content_ = std::move(a_content);
		return *this;
	}
	
    HttpResponse& SetCode(HttpCode a_code)
	{
		code_ = a_code;
		comment_ = comment_map_.at(code_);
		return *this;
	}

	friend std::ostream& operator << (std::ostream& output, const HttpResponse& resp)
	{
		output << prefix_ << " " << static_cast<int>(resp.code_) << " " << resp.comment_ << '\n';

        for (const auto& [name, value] : resp.headers_)
        {
			output << name << ": " << value << '\n';
        }

        if (!resp.content_.empty())
        {
			output << "Content-Length: " << resp.content_.size() << '\n' << '\n';
			output << resp.content_;
        }

        if (resp.content_.empty())
		    output << '\n';
		
	    return output;
	}

private:
	inline const static std::string prefix_ = "HTTP/1.1";
	inline const static std::unordered_map<HttpCode, std::string> comment_map_ = {
		{HttpCode::Ok, "OK"},
		{HttpCode::Found, "Found"},
		{HttpCode::NotFound, "Not found"} };
	
    HttpCode code_;
	std::string comment_;
    std::unordered_multimap<std::string, std::string> headers_;
	std::string content_;
};


std::pair<std::string, std::string> SplitBy(const std::string& what, const std::string& by);


void TestCommentServer();