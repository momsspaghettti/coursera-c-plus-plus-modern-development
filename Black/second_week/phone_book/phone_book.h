#pragma once
#include "iterator_range.h"
#include "contact.pb.h"
#include <optional>
#include <string>
#include <string_view>
#include <vector>
#include <iosfwd>
#include <set>


struct Date {
    int year, month, day;
};

struct Contact {
    std::string name;
    std::optional<Date> birthday;
    std::vector<std::string> phones;
};


bool operator<(const Contact&, const Contact&);


class PhoneBook {
public:
    explicit PhoneBook(const std::vector<Contact>& contacts);

    using ContactsIterator = std::multiset<Contact>::const_iterator;
    using ContactRange = IteratorRange<ContactsIterator>;
    [[nodiscard]] ContactRange FindByNamePrefix(std::string_view name_prefix) const;

    void SaveTo(std::ostream& output) const;

private:
    std::multiset<Contact> contacts_;
};

PhoneBook DeserializePhoneBook(std::istream& input);

void TestPhoneBook();