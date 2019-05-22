#include "INotifier.h"
#include <iostream>


SmsNotifier::SmsNotifier (const std::string& number) : number_(number){}


void SmsNotifier::Notify(const std::string& message)
{
	SendSms(number_, message);
}


EmailNotifier::EmailNotifier(const std::string& email) : email_(email){}


void EmailNotifier::Notify(const std::string& message)
{
	SendEmail(email_, message);
}


void SendSms(const std::string& number, const std::string& message) {
    std::cout << "Send '" << message << "' to number " << number << std::endl;
}


void SendEmail(const std::string& email, const std::string& message) {
    std::cout << "Send '" << message << "' to e-mail " << email << std::endl;
}