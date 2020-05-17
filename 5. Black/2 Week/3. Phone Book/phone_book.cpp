//
// Created by ilya_l on 5/17/20.
//

#include "phone_book.h"

bool operator<(const Contact& lhs, const Contact& rhs) {
  return lhs.name < rhs.name;
}

PhoneBook::PhoneBook(std::vector<Contact> contacts) {
  contacts_.reserve(contacts.size());
  contacts_ = std::move(contacts);
  std::sort(std::begin(contacts_), std::end(contacts_));
}

PhoneBook::ContactRange PhoneBook::FindByNamePrefix(std::string_view name_prefix) const {
  if (name_prefix == "") return ContactRange(std::begin(contacts_), std::end(contacts_));
  auto iterator = std::lower_bound(std::begin(contacts_), std::end(contacts_), Contact{.name = name_prefix.data()});
  if (iterator == std::end(contacts_)) return ContactRange(std::end(contacts_), std::end(contacts_));
  if (!PrefixEqual(name_prefix, iterator->name)) return ContactRange(std::end(contacts_), std::end(contacts_));

  auto end_iterator = std::next(iterator);
  while(end_iterator != std::end(contacts_) && PrefixEqual(name_prefix, end_iterator->name)) {
    end_iterator = std::next(end_iterator);
  }
  return ContactRange(iterator, end_iterator);
}

void PhoneBook::SaveTo(std::ostream& output) const {
  PhoneBookSerialize::ContactList pb_contacts;
  for (const auto& contact : contacts_) {
    PhoneBookSerialize::Contact pb_contact;
    pb_contact.set_name(contact.name);
    if (contact.birthday) {
      PhoneBookSerialize::Date pb_date;
      pb_date.set_day(contact.birthday->day);
      pb_date.set_month(contact.birthday->month);
      pb_date.set_year(contact.birthday->year);
      *pb_contact.mutable_birthday() = pb_date;
    }
    if (!contact.phones.empty()) {
      for (const auto& number : contact.phones) {
        pb_contact.add_phone_number(number);
      }
    }
    *pb_contacts.add_contact() = pb_contact;
  }
  pb_contacts.SerializePartialToOstream(&output);
}

bool PhoneBook::PrefixEqual(std::string_view prefix, const std::string& str) {
  size_t size = std::min(prefix.size(), str.size());
  for (size_t i = 0; i < size; ++i) {
    if (str[i] != prefix[i]) {
      return false;
    }
  }
  return true;
}

PhoneBook DeserializePhoneBook(std::istream& input) {
  PhoneBookSerialize::ContactList pb_contacts_;
  pb_contacts_.ParseFromIstream(&input);
  std::vector<Contact> contacts;
  contacts.reserve(pb_contacts_.contact_size());
  for (const auto& pb_contact : pb_contacts_.contact()) {
    Contact contact;
    contact.name = pb_contact.name();
    if (pb_contact.has_birthday()) {
      contact.birthday = {
          .year = pb_contact.birthday().year(),
          .month = pb_contact.birthday().month(),
          .day = pb_contact.birthday().day()};
    }
    contact.phones.reserve(pb_contact.phone_number_size());
    for (const auto& pb_number : pb_contact.phone_number()) {
      contact.phones.push_back(pb_number);
    }
    contacts.push_back(contact);
  }
  return PhoneBook(contacts);
}