#pragma once
#include <string>
#include <memory>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <functional>


struct Email
{
	std::string from;
	std::string to;
    std::string body;
};


inline std::istream& operator>>(std::istream& input, Email& email)
{
	std::string from, to, body;

	std::getline(input, from);
	std::getline(input, to);
	std::getline(input, body);

	email = Email{std::move(from), std::move(to), std::move(body)};

	return input;
}


inline std::ostream& operator<<(std::ostream& output, const Email& email)
{
	output << email.from << '\n';
	output << email.to << '\n';
	output << email.body << '\n';

	return output;
}


class Worker 
{
public:	
    virtual void Process(std::unique_ptr<Email> email) = 0;
	
    virtual void Run() 
    {
		// только первому worker-у в пайплайне нужно это имплементировать
		throw std::logic_error("Unimplemented");
	}

	void SetNext(std::unique_ptr<Worker> next)
    {
		if (!next_)
		{
			next_ = std::move(next);
		}
        else
        {
			next_->SetNext(std::move(next));
        }
    }

	virtual ~Worker() = default;

protected:
	// реализации должны вызывать PassOn, чтобы передать объект дальше
	// по цепочке обработчиков
	void PassOn(std::unique_ptr<Email> email) const
	{
		if (next_)
			next_->Process(std::move(email));
	}

private:
	std::unique_ptr<Worker> next_ = nullptr;
};


class Reader : public Worker 
{
public:
    explicit Reader(std::istream& input)
    {
		Email email;
        while (input >> email)
        {
			emails_.emplace_back(std::make_unique<Email>(std::move(email)));
        }
    }

    void Run() override
    {
        for (std::unique_ptr<Email>& email : emails_)
        {
			PassOn(std::move(email));
        }
    }

    void Process(std::unique_ptr<Email> email) override
    {
		Run();
    }

private:
	std::vector<std::unique_ptr<Email>> emails_;
};


class Filter : public Worker 
{
public:
	using Function = std::function<bool(const Email&)>;

	explicit Filter(const Function& function) : function_(function) {}

    void Process(std::unique_ptr<Email> email) override
	{
		if (function_(*email))
			PassOn(std::move(email));
	}

private:
	const Function function_;
};


class Copier : public Worker
{
public:
	explicit Copier(const std::string& recipient) : recipient_(recipient) {}

    void Process(std::unique_ptr<Email> email) override
    {
		std::unique_ptr<Email> new_email = nullptr;
		if (recipient_ != email->to)
		{
			new_email = std::make_unique<Email>(Email{email->from, recipient_, email->body});
		}

		PassOn(std::move(email));

		if (new_email)
			PassOn(std::move(new_email));
    }

private:
	const std::string recipient_;
};


class Sender : public Worker
{
public:
	explicit Sender(std::ostream& output) : output_(output) {}

    void Process(std::unique_ptr<Email> email) override
    {
		output_ << *email;
		PassOn(std::move(email));
    }

private:
	std::ostream& output_;
};


// реализуйте класс
class PipelineBuilder 
{
public:
	// добавляет в качестве первого обработчика Reader
	explicit PipelineBuilder(std::istream& in)
	{
		head_ = std::make_unique<Reader>(in);
	}

	// добавляет новый обработчик Filter
	PipelineBuilder& FilterBy(Filter::Function filter)
	{
		head_->SetNext(std::make_unique<Filter>(std::move(filter)));

		return *this;
	}

	// добавляет новый обработчик Copier
	PipelineBuilder& CopyTo(std::string recipient)
	{
		head_->SetNext(std::make_unique<Copier>(recipient));

		return *this;
	}

	// добавляет новый обработчик Sender
	PipelineBuilder& Send(std::ostream& out)
	{
		head_->SetNext(std::make_unique<Sender>(out));

		return *this;
	}

	// возвращает готовую цепочку обработчиков
    std::unique_ptr<Worker> Build()
	{
		return std::move(head_);
	}

private:
	std::unique_ptr<Worker> head_;
};


void TestPipeLine();