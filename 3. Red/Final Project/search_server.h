#pragma once

#include <istream>
#include <ostream>
#include <set>
#include <list>
#include <vector>
#include <map>
#include <string>

#include <deque>

using namespace std;

class InvertedIndex {
public:
  void Add(string &&document);
  list<size_t> Lookup(string_view word) const;
  std::map<size_t, size_t> LookupFreq(string_view word) const;

  const string& GetDocument(size_t id) const {
    return docs[id];
  }

private:
  // Хранить string_view вместо string
  // т.к. document хранится в docs и строки не исчезают
  map<std::string_view, list<size_t>> index;
  map<std::string_view, std::map<size_t, size_t>> freq_index;
  std::deque<string> docs;
};

class SearchServer {
public:
  SearchServer() = default;
  explicit SearchServer(istream& document_input);
  void UpdateDocumentBase(istream& document_input);
  void AddQueriesStream(istream& query_input, ostream& search_results_output);

private:
  InvertedIndex index;
};
