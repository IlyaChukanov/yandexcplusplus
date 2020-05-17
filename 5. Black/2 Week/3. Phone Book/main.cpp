#include "phone_book.h"
#include "contact.pb.h"

#include <test_runner.h>

#include <sstream>

using namespace std;


void TestSerialization() {
  const PhoneBook ab({
    {"Ivan Ivanov", Date{1980, 1, 13}, {"+79850685521"}},
    {"Margarita Petrova", Date{1989, 4, 23}, {"+79998887766", "+71112223344"}},
    {"Just Birthday", Date{1989, 4, 23}, {}},
    {"No Birthday", std::nullopt, {"+7-4862-77-25-64"}},
  });

  ostringstream output(std::ios::binary);
  ab.SaveTo(output);

  istringstream input(output.str(), std::ios::binary);
  PhoneBookSerialize::ContactList list;
  ASSERT(list.ParseFromIstream(&input));
  ASSERT_EQUAL(list.contact_size(), 4);

  unordered_map<string, const PhoneBookSerialize::Contact*> persons;
  for (const auto& p : list.contact()) {
    persons[p.name()] = &p;
  }

  {
    auto it = persons.find("Ivan Ivanov");
    ASSERT(it != persons.end());
    const auto& p = *it->second;

    ASSERT(p.has_birthday());
    ASSERT_EQUAL(p.birthday().year(), 1980);
  }
  {
    auto it = persons.find("Margarita Petrova");
    ASSERT(it != persons.end());
    const auto& p = *it->second;

    ASSERT(p.has_birthday());
    ASSERT_EQUAL(p.birthday().year(), 1989);
    ASSERT_EQUAL(p.phone_number_size(), 2);
    ASSERT_EQUAL(p.phone_number(1), "+71112223344");
  }
  {
    auto it = persons.find("Just Birthday");
    ASSERT(it != persons.end());
    const auto& p = *it->second;

    ASSERT(p.has_birthday());
    ASSERT_EQUAL(p.birthday().day(), 23);
    ASSERT_EQUAL(p.phone_number_size(), 0);
  }
  {
    auto it = persons.find("No Birthday");
    ASSERT(it != persons.end());
    const auto& p = *it->second;

    ASSERT(!p.has_birthday());
    ASSERT_EQUAL(p.phone_number_size(), 1);
  }
}

void TestDeserialization() {
  PhoneBookSerialize::ContactList list;

  PhoneBookSerialize::Contact p;
  p.set_name("Vanya");
  p.add_phone_number("+7-953-181-46-12");

  *list.add_contact() = p;

  p.Clear();
  p.set_name("Veronika");
  p.mutable_birthday()->set_year(2000);
  p.mutable_birthday()->set_month(3);
  p.mutable_birthday()->set_day(31);
  p.add_phone_number("325-87-16");

  *list.add_contact() = p;

  p.Clear();
  p.set_name("Vitya");

  *list.add_contact() = p;

  ostringstream output(std::ios::binary);
  list.SerializeToOstream(&output);

  istringstream input(output.str(), std::ios::binary);
  const PhoneBook book = DeserializePhoneBook(input);

  auto range = book.FindByNamePrefix("");
  vector<Contact> contacts(range.begin(), range.end());

  ASSERT_EQUAL(contacts.size(), 3);

  ASSERT_EQUAL(contacts[0].name, "Vanya");
  ASSERT_EQUAL(contacts[0].phones, (vector<string>{"+7-953-181-46-12"}));
  ASSERT(!contacts[0].birthday);

  ASSERT_EQUAL(contacts[1].name, "Veronika");
  ASSERT(contacts[1].birthday.has_value());
  ASSERT_EQUAL(contacts[1].birthday->year, 2000);
  ASSERT_EQUAL(contacts[1].birthday->month, 3);
  ASSERT_EQUAL(contacts[1].birthday->day, 31);
  ASSERT_EQUAL(contacts[1].phones, (vector<string>{"325-87-16"}));

  ASSERT_EQUAL(contacts[2].name, "Vitya");
  ASSERT(!contacts[2].birthday);
  ASSERT(contacts[2].phones.empty());
}


void TestFindNameByPrefix() {
  PhoneBook book({
    {"Vasiliy Petrov", std::nullopt, {}},
    {"Ivan Ivanov", std::nullopt, {}},
    {"Vasiliy Ivanov", std::nullopt, {}},
    {"Vasilisa Kuznetsova", std::nullopt, {}},
    {"Ivan Petrov", std::nullopt, {}},
    {"Vassisualiy Lokhankin", std::nullopt, {}},
    {"Ivan Vasiliev", std::nullopt, {}},
    {"", std::nullopt, {}},
  });

  auto get_names = [](PhoneBook::ContactRange range) {
    vector<string> result;
    for (const auto& record : range) {
      result.push_back(record.name);
    }
    return result;
  };
  ASSERT_EQUAL(
    get_names(book.FindByNamePrefix("Ivan")),
    (vector<string>{"Ivan Ivanov", "Ivan Petrov", "Ivan Vasiliev"})
  );
  ASSERT_EQUAL(
    get_names(book.FindByNamePrefix("I")),
    (vector<string>{"Ivan Ivanov", "Ivan Petrov", "Ivan Vasiliev"})
  );
  ASSERT_EQUAL(
    get_names(book.FindByNamePrefix("Iv")),
    (vector<string>{"Ivan Ivanov", "Ivan Petrov", "Ivan Vasiliev"})
  );
  ASSERT_EQUAL(
    get_names(book.FindByNamePrefix("Ivan Ivan")), (vector<string>{"Ivan Ivanov"})
  );
  ASSERT_EQUAL(
    get_names(book.FindByNamePrefix("Vas")),
    (vector<string>{
      "Vasilisa Kuznetsova",
      "Vasiliy Ivanov",
      "Vasiliy Petrov",
      "Vassisualiy Lokhankin"
    })
  );
  ASSERT_EQUAL(
    get_names(book.FindByNamePrefix("Vasili")),
    (vector<string>{"Vasilisa Kuznetsova", "Vasiliy Ivanov", "Vasiliy Petrov"})
  );
  ASSERT_EQUAL(
    get_names(book.FindByNamePrefix("Vasiliy")),
    (vector<string>{"Vasiliy Ivanov", "Vasiliy Petrov"})
  );
  ASSERT_EQUAL(get_names(book.FindByNamePrefix("Vasilis")), (vector<string>{"Vasilisa Kuznetsova"}));
  ASSERT_EQUAL(
    get_names(book.FindByNamePrefix("")),
    (vector<string>{
      "",
      "Ivan Ivanov",
      "Ivan Petrov",
      "Ivan Vasiliev",
      "Vasilisa Kuznetsova",
      "Vasiliy Ivanov",
      "Vasiliy Petrov",
      "Vassisualiy Lokhankin"
    })
  );
}


void TestFindNameByPrefix2() {
  PhoneBook book({
    {"a", std::nullopt, {}},
    {"aaaa", std::nullopt, {}},
    {"aabc", std::nullopt, {}},
    {"aabccc", std::nullopt, {}},
    {"aabcbc", std::nullopt, {}},
    {"aeca", std::nullopt, {}},
    {"aeca", std::nullopt, {}},
    {"aefg", std::nullopt, {}},
    {"aq", std::nullopt, {}},
  });

  auto get_names = [](PhoneBook::ContactRange range) {
    vector<string> result;
    for (const auto& record : range) {
      result.push_back(record.name);
    }
    return result;
  };

  ASSERT_EQUAL(book.FindByNamePrefix("a").size(), 9u);
  ASSERT_EQUAL(
    get_names(book.FindByNamePrefix("aa")),
    (vector<string>{"aaaa", "aabc", "aabcbc", "aabccc"})
  );

  ASSERT_EQUAL(book.FindByNamePrefix("ab").size(), 0u);
  ASSERT_EQUAL(book.FindByNamePrefix("b").size(), 0u);
  ASSERT_EQUAL(book.FindByNamePrefix("aaaaa").size(), 0u);
  ASSERT_EQUAL(book.FindByNamePrefix("aeb").size(), 0u);
  ASSERT_EQUAL(book.FindByNamePrefix("aed").size(), 0u);
  ASSERT_EQUAL(book.FindByNamePrefix("aeg").size(), 0u);

  ASSERT_EQUAL(get_names(book.FindByNamePrefix("aaa")), (vector<string>{"aaaa"}));
  ASSERT_EQUAL(
    get_names(book.FindByNamePrefix("ae")),
    (vector<string>{"aeca", "aeca", "aefg"})
  );
}


int main() {
  TestRunner tr;
  RUN_TEST(tr, TestFindNameByPrefix);
  RUN_TEST(tr, TestFindNameByPrefix2);
  RUN_TEST(tr, TestSerialization);
  RUN_TEST(tr, TestDeserialization);
  std::string a = "Compile error: bIn file included from /tmp/submissionxp6couh2/phone_book.cpp:5:\nIn file included from /tmp/submissionxp6couh2/phone_book.h:15:\nIn file included from /tmp/tmp66u0kjt7/contact.pb.h:25:\nIn file included from /usr/include/google/protobuf/generated_message_table_driven.h:34:\nIn file included from /usr/include/google/protobuf/map.h:49:\nIn file included from /usr/include/google/protobuf/map_type_handler.h:35:\nIn file included from /usr/include/google/protobuf/wire_format_lite_inl.h:44:\n/usr/include/google/protobuf/repeated_field.h:1554:32: fatal error: too many arguments to function call, expected single argument 'arena', have 2 arguments\n      TypeHandler::New(arena_, std::move(value));\n      ~~~~~~~~~~~~~~~~         ^~~~~~~~~~~~~~~~\n/usr/include/google/protobuf/repeated_field.h:1982:25: note: in instantiation of function template specialization 'google::protobuf::internal::RepeatedPtrFieldBase::Add<google::protobuf::RepeatedPtrField<PhoneBookSerialize::Contact>::TypeHandler>' requested here\n  RepeatedPtrFieldBase::Add<TypeHandler>(std::move(value));\n                        ^\n/tmp/submissionxp6couh2/phone_book.cpp:47:36: note: in instantiation of member function 'google::protobuf::RepeatedPtrField<PhoneBookSerialize::Contact>::Add' requested here\n    pb_contacts.mutable_contact()->Add(std::move(pb_contact));\n                                   ^\n/usr/include/google/protobuf/repeated_field.h:642:3: note: 'New' declared here\n  static inline GenericType* New(Arena* arena) {\n  ^\n1 error generated.\n";
  std::cout << a << std::endl;
}
