//
// Created by ilya_l on 5/17/20.
//

#ifndef YANDEXCPLUSPLUS_5_BLACK_2_WEEK_3_PHONE_BOOK_PHONE_BOOK_H
#define YANDEXCPLUSPLUS_5_BLACK_2_WEEK_3_PHONE_BOOK_PHONE_BOOK_H
#include <optional>
#include <string>
#include <string_view>
#include <vector>
#include <set>
#include <iosfwd>

#include "iterator_range.h"
#include "contact.pb.h"

struct Date {
  int year, month, day;
};

struct Contact {
  std::string name;
  std::optional<Date> birthday;
  std::vector<std::string> phones;
  friend bool operator<(const Contact& lhs, const Contact& rhs);
};

class PhoneBook {
public:
  using ContactRange = IteratorRange<std::vector<Contact>::const_iterator>;
  explicit PhoneBook(std::vector<Contact> contacts);
  ContactRange FindByNamePrefix(std::string_view name_prefix) const;
  void SaveTo(std::ostream& output) const;
private:
  static bool PrefixEqual(std::string_view prefix, const std::string& str);
  std::vector<Contact> contacts_;
};
PhoneBook DeserializePhoneBook(std::istream& input);

#endif //YANDEXCPLUSPLUS_5_BLACK_2_WEEK_3_PHONE_BOOK_PHONE_BOOK_H
