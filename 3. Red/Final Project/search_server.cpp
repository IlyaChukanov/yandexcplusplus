#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include <vector>

vector<std::string_view> SplitIntoWords(string_view line) {
  std::vector<std::string_view> result;
  size_t curr = 0;
  while (true) {
    auto space = line.find(' ', curr);
    result.emplace_back(line.substr(curr, space - curr));
    if (space == line.npos)
      break;
    else
      curr = (space + 1);
  }
  return result;
}

SearchServer::SearchServer(istream& document_input) {
  UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
  InvertedIndex new_index;
  for (string current_document; getline(document_input, current_document); ) {
    new_index.Add(move(current_document));
  }
  index = move(new_index);
}

void SearchServer::AddQueriesStream(istream& query_input, ostream& search_results_output) {
  for (string current_query; getline(query_input, current_query); ) {

    /*
    map<size_t, size_t> docid_count;
    for (const auto& word : SplitIntoWords(current_query)) {
      for (const size_t docid : index.Lookup(word)) {
        docid_count[docid]++;
      }
    }
    */
    deque<pair<size_t, size_t>> search_results;
    for (const auto& word : SplitIntoWords(current_query)) {
      for (const auto& map_id : index.LookupFreq(word)) {
        search_results.push_back(map_id);
      }
    }
    /*
    vector<pair<size_t, size_t>> search_results(
        std::make_move_iterator(docid_count.begin()), std::make_move_iterator(docid_count.end())
    );
    */
    sort(
      begin(search_results),
      end(search_results),
      [](pair<size_t, size_t> lhs, pair<size_t, size_t> rhs) {
        int64_t lhs_docid = lhs.first;
        auto lhs_hit_count = lhs.second;
        int64_t rhs_docid = rhs.first;
        auto rhs_hit_count = rhs.second;
        return make_pair(lhs_hit_count, -lhs_docid) > make_pair(rhs_hit_count, -rhs_docid);
      }
    );

    search_results_output << current_query << ':';
    for (auto [docid, hitcount] : Head(search_results, 5)) {
      search_results_output << " {"
        << "docid: " << docid << ", "
        << "hitcount: " << hitcount << '}';
    }
    search_results_output << endl;
  }
}

void InvertedIndex::Add(string &&document) {
  docs.push_back(std::move(document));
  const size_t docid = docs.size() - 1;
  for (const auto& word : SplitIntoWords(docs.back())) {
    index[word].push_back(docid);
    //freq_index[word][docid]++;
  }
}

list<size_t> InvertedIndex::Lookup(string_view word) const {
  if (auto it = index.find(word); it != index.end()) {
    return it->second;
  } else {
    return {};
  }
}

std::map<size_t, size_t> InvertedIndex::LookupFreq(string_view word) const {
  if (auto it = freq_index.find(word); it != freq_index.end()) {
    return it->second;
  } else {
    return {};
  }
}