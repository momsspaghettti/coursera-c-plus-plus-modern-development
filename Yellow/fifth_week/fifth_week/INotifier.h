#pragma once
#include <string>


class INotifier
{
public:
	virtual void Notify(const std::string& message) = 0;
};


class SmsNotifier : public INotifier
{
public:
	SmsNotifier(const std::string& number);

    void Notify(const std::string& message) override;

private:
	const std::string number_;
};


class EmailNotifier : public INotifier
{
public:
	EmailNotifier(const std::string& email);

    void Notify(const std::string& message) override;

private:
	const std::string email_;
};


void SendSms(const std::string& number, const std::string& message);
void SendEmail(const std::string& email, const std::string& message);