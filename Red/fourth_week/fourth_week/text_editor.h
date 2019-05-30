#pragma once
#include <string>
#include <list>


class Editor
{
public:
	Editor();

	void Left();

	void Right();

	void Insert(char token);

	void Cut(size_t tokens = 1);

	void Copy(size_t tokens = 1);

	void Paste();

	std::string GetText() const;

private:
	std::list<char> text_;
	std::list<char> buffer_;
	std::list<char>::iterator cursor_;

	void Deleter(const std::list<char>::iterator& l);
	void Inserter(const std::list<char>::iterator& l);
	std::list<char>::iterator ComputeCursor(size_t tokens);
};


inline void Editor::Deleter(const std::list<char>::iterator& l)
{
	text_.erase(l, cursor_);
}


inline void Editor::Inserter(const std::list<char>::iterator& l)
{
	buffer_.clear();
	buffer_.insert(buffer_.end(), l, cursor_);
}


inline std::list<char>::iterator Editor::ComputeCursor(size_t tokens)
{
	const size_t dist_to_end = std::distance(cursor_, text_.end());
	const auto tmp_iter = cursor_;
	tokens >= dist_to_end ? cursor_ = text_.end() : cursor_ = std::next(cursor_, tokens);

	return tmp_iter;
}


inline Editor::Editor()
{
	cursor_ = text_.begin();
}


inline void Editor::Left()
{
    if (cursor_ != text_.begin())
    {
		cursor_ = std::prev(cursor_);
    }
}


inline void Editor::Right()
{
    if (cursor_ != text_.end())
    {
		cursor_ = std::next(cursor_);
    }
}


inline void Editor::Insert(char token)
{
	text_.insert(cursor_, token);
}


inline void Editor::Cut(size_t tokens)
{
	const auto tmp_iter = ComputeCursor(tokens);

	Inserter(tmp_iter);
	Deleter(tmp_iter);
}


inline void Editor::Copy(size_t tokens)
{
	const auto tmp_iter = ComputeCursor(tokens);
	Inserter(tmp_iter);
}


inline void Editor::Paste()
{
    if (!buffer_.empty())
    {
		text_.insert(cursor_, buffer_.begin(), buffer_.end());
    }
}


inline std::string Editor::GetText() const
{
	return std::string(text_.begin(), text_.end());
}


void TestTextEditor();