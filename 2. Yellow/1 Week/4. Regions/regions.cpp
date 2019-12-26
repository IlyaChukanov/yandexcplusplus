//
// Created by ilya on 26.08.2019.
//

#include <vector>
#include <map>
#include <iostream>
#include <string>

using namespace std;

enum class Lang {
  DE, FR, IT
};


struct Region {
  string std_name;
  string parent_std_name;
  map<Lang, string> names;
  int64_t population;
};

bool operator<(const Region& lhs, const Region& rhs) {
  return std::tie(lhs.std_name, lhs.parent_std_name, lhs.names, lhs.population) <
      std::tie(rhs.std_name, rhs.parent_std_name, rhs.names, rhs.population);
}

int FindMaxRepetitionCount(const vector<Region>& regions) {
  int result = 0;
  map<Region, int> repetion_count;
  for (const Region& region : regions) {
    result = max(result, ++repetion_count[region]);
  }
  return result;
}

int main() {
  cout << FindMaxRepetitionCount({
                                     {
                                         "Moscow",
                                         "Russia",
                                         {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
                                         89
                                     }, {
                                         "Russia",
                                         "Eurasia",
                                         {{Lang::DE, "Russland"}, {Lang::FR, "Russie"}, {Lang::IT, "Russia"}},
                                         89
                                     }, {
                                         "Moscow",
                                         "Russia",
                                         {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
                                         89
                                     }, {
                                         "Moscow",
                                         "Russia",
                                         {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
                                         89
                                     }, {
                                         "Russia",
                                         "Eurasia",
                                         {{Lang::DE, "Russland"}, {Lang::FR, "Russie"}, {Lang::IT, "Russia"}},
                                         89
                                     },
                                 }) << endl;

  cout << FindMaxRepetitionCount({
                                     {
                                         "Moscow",
                                         "Russia",
                                         {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
                                         89
                                     }, {
                                         "Russia",
                                         "Eurasia",
                                         {{Lang::DE, "Russland"}, {Lang::FR, "Russie"}, {Lang::IT, "Russia"}},
                                         89
                                     }, {
                                         "Moscow",
                                         "Russia",
                                         {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou deux"}, {Lang::IT, "Mosca"}},
                                         89
                                     }, {
                                         "Moscow",
                                         "Toulouse",
                                         {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
                                         89
                                     }, {
                                         "Moscow",
                                         "Russia",
                                         {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
                                         31
                                     },
                                 }) << endl;

  return 0;
}
