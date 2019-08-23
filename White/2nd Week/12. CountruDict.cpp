//
// Created by ilya on 16.07.19.
//

#include <iostream>
#include <string>
#include <map>


void ChangeCapital(std::map<std::string, std::string> &country_dict, std::string &country, std::string &new_capital) {
  if (country_dict.count(country) == 0) {
    std::cout << "Introduce new country " << country << " with capital " << new_capital << std::endl;
    country_dict[country] = new_capital;
  }
  else if (country_dict.count(country) && country_dict[country] == new_capital) {
    std::cout << "Country " << country << " hasn't changed its capital" << std::endl;
  }
  else {
    std::cout << "Country " << country << " has changed its capital from " << country_dict[country]
              << " to " << new_capital << std::endl;
    country_dict[country] = new_capital;
  }
}

void Rename(std::map<std::string, std::string> &country_dict, std::string &old_country_name, std::string &new_country_name) {

  bool check1 = old_country_name == new_country_name;
  bool check2 = !country_dict.count(old_country_name);
  bool check3 = country_dict.count(new_country_name);
  bool res = check1 || check2 || check3;
  if (res) {
    std::cout << "Incorrect rename, skip" << std::endl;
  }
  else {
    std::cout << "Country " << old_country_name << " with capital " << country_dict[old_country_name]
              << " has been renamed to " << new_country_name << std::endl;
    country_dict[new_country_name] = country_dict[old_country_name];
    country_dict.erase(old_country_name);
  }
}

void About(const std::map<std::string, std::string> &country_dict, const std::string &country) {

  if (country_dict.count(country) == 0) {
    std::cout << "Country " << country << " doesn't exist" << std::endl;
  }
  else {
    std::cout << "Country " << country << " has capital " << country_dict.at(country) << std::endl;
  }
}

void Dump(const std::map<std::string, std::string> &country_dict) {
  if (country_dict.empty()) {
    std::cout << "There are no countries in the world" << std::endl;
  }
  else {
    for (auto item : country_dict) {
      std::cout << item.first << "/" << item.second << std::endl;
    }
  }
}

int main() {


  std::map<std::string, std::string> country_dict;

  int q;
  std::cin >> q;
  for (int i = 0; i < q; ++i) {
    std::string request;
    std::cin >> request;
    if (request == "CHANGE_CAPITAL") {
      std::string country, new_capital;
      std::cin >> country >> new_capital;
      ChangeCapital(country_dict, country, new_capital);
    }

    if (request == "RENAME") {
      std::string old_country_name, new_country_name;
      std::cin >> old_country_name >> new_country_name;
      Rename(country_dict, old_country_name, new_country_name);
    }

    if (request == "ABOUT") {
      std::string country;
      std::cin >> country;
      About(country_dict, country);
    }

    if (request == "DUMP") {
      Dump(country_dict);
    }
  }

  return 0;
}