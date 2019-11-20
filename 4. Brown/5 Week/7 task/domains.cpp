//
// Created by ilya on 20.11.2019.
//

#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <algorithm>

struct ParsedDomain {
  std::string main_domain;
  std::string zone;
  std::vector<std::string> sub_domain;
};

std::vector<std::string> ReadFullDomains(std::istream& stream) {
  int count;
  stream >> count;
  std::vector<std::string> res(count);
  for (auto& s : res) {
    stream >> s;
  }
  return res;
}

class DomainFilter {
 public:
  using DomainsContainer = std::unordered_map<std::string, std::unordered_map<std::string, std::vector<ParsedDomain>>>;
  explicit DomainFilter(const std::vector<std::string>& raw_domains) {
    ParseFullDomains(raw_domains);
  }

  // true - можно пропустить
  // false - пропустить нельзя
  bool FilterDomain(const std::string& dom) {
    auto domain = ParseFullDomain(dom);
    // фильтр по зоне
    // *zone
    if (!bad_domains_.count(domain.zone)) {
      return true;
    }
    // фильтр по главному домену
    // *main_domain.zone
    if (bad_domains_[domain.zone].count("")) {
      return false;
    }
    if (!bad_domains_[domain.zone].count(domain.main_domain)) {
      return true;
    }
    // фильтр по доп. доменам
    for (const auto& bad : bad_domains_[domain.zone][domain.main_domain]) {
      if (bad.sub_domain.empty() || domain.sub_domain.empty()) {
        return false;
      }
      auto curr_dom = domain.sub_domain.begin();
      auto dom_last = domain.sub_domain.end();
      auto curr_bad = bad.sub_domain.begin();
      auto bad_last = bad.sub_domain.end();
      for(;;++curr_dom, ++curr_bad) {
        if (*curr_bad != *curr_dom) {
          break;
        }
        else if (curr_dom == dom_last && curr_bad == bad_last) {
          return false;
        }
        else if (curr_dom == dom_last) {
          break;
        }
        else if (curr_bad == bad_last) {
          return false;
        }
      }
    }
    return true;
  }

 private:

  //std::vector<ParsedDomain> bad_domains;
  DomainsContainer bad_domains_;
  void ParseFullDomains(const std::vector<std::string>& raw_domains) {
    for (const auto& domain : raw_domains) {
      auto pd = ParseFullDomain(domain);
      bad_domains_[pd.zone][pd.main_domain].push_back(std::move(pd));
    }
  }

  static ParsedDomain ParseFullDomain(const std::string& str) {
    auto domains = SplitBy(str);
    ParsedDomain pd;
    pd.zone = domains.back();
    domains.pop_back();
    if (!domains.empty()) {
      pd.main_domain = domains.back();
      domains.pop_back();
    }
    for (auto i = domains.rbegin(); i != domains.rend() && !domains.empty(); ++i) {
      pd.sub_domain.emplace_back(*i);
    }
    return pd;
  }

  static std::vector<std::string> SplitBy(const std::string & s, char split = '.') {
    std::vector<std::string> result;
    auto curr = s.begin();
    while (curr != s.end()) {
      auto space = std::find(curr, s.end(), split);
      result.emplace_back(std::string(curr, space));
      if (space != s.end()) curr = (space + 1);
      else curr = s.end();
    }
    return result;
  }
};

int main() {
  auto bad_domains = ReadFullDomains(std::cin);
  auto domains = ReadFullDomains(std::cin);
  DomainFilter filter(bad_domains);
  for (const auto& d : domains) {
    auto res = filter.FilterDomain(d);
    if (res) {
      std::cout << "Good" << std::endl;
    }
    else {
      std::cout << "Bad" << std::endl;
    }
  }
  return 0;
}
