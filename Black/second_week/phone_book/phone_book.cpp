#include "phone_book.h"


bool operator<(const Contact& lhs, const Contact& rhs) {
    return lhs.name < rhs.name;
}


PhoneBook::PhoneBook(const std::vector<Contact>& contacts) {
    for (const auto& contact : contacts) {
        contacts_.insert(contact);
    }
}


PhoneBook::ContactRange PhoneBook::FindByNamePrefix(std::string_view name_prefix) const {
    if (!name_prefix.empty()) {
        const Contact finder1{std::string(name_prefix)};

        std::string tmp = std::string(name_prefix);
        tmp[tmp.size() - 1] = char(tmp[tmp.size() - 1] + 1);
        const Contact finder2{tmp};

        const auto lower1 = contacts_.lower_bound(finder1);
        const auto lower2 = contacts_.lower_bound(finder2);

        return IteratorRange(lower1, lower2);
    } else {
        return IteratorRange(contacts_.begin(), contacts_.end());
    }
}


void PhoneBook::SaveTo(std::ostream &output) const {
    PhoneBookSerialize::ContactList list;

    for (const auto& contact : contacts_) {
        PhoneBookSerialize::Contact* serialize_contact = list.add_contact();
        *serialize_contact->mutable_name() = contact.name;

        if (contact.birthday) {
            serialize_contact->mutable_birthday()->set_day(contact.birthday->day);
            serialize_contact->mutable_birthday()->set_month(contact.birthday->month);
            serialize_contact->mutable_birthday()->set_year(contact.birthday->year);
        }

        for (const auto& number : contact.phones) {
            serialize_contact->add_phone_number(number);
        }
    }

    list.SerializePartialToOstream(&output);
}


PhoneBook DeserializePhoneBook(std::istream& input) {
    PhoneBookSerialize::ContactList list;

    if (list.ParseFromIstream(&input)) {
        std::vector<Contact> contacts;
        contacts.reserve(list.contact_size());

        for (const auto& des_contact : *list.mutable_contact()) {
            Contact contact;
            contact.name = des_contact.name();

            if (des_contact.has_birthday()) {
                contact.birthday = Date{
                        des_contact.birthday().year(),
                        des_contact.birthday().month(),
                        des_contact.birthday().day()
                };
            }

            for (const auto& des_number : des_contact.phone_number()) {
                contact.phones.push_back(des_number);
            }

            contacts.push_back(contact);
        }

        return PhoneBook(contacts);
    } else {
        throw std::runtime_error("Deserialization Error!");
    }
}