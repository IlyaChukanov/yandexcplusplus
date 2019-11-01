#include "search_server.h"
#include "iterator_range.h"
#include "../../MyUtils/Profiler/profiler.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include <vector>

vector<std::string_view> SplitIntoWords(string_view line) {
  std::vector<std::string_view> result;
  size_t curr = line.find_first_not_of(' ', 0);
  while (true) {
    auto space = line.find(' ', curr);
    result.emplace_back(line.substr(curr, space - curr));
    if (space == line.npos)
      break;
    else
      curr = line.find_first_not_of(' ', space);
      if (curr == line.npos)
        break;
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
  std::swap(new_index, index);
}

void SearchServer::AddQueriesStream(istream& query_input, ostream& search_results_output) {
  TotalDuration parse("Total parse query");
  TotalDuration move("Total move data");
  TotalDuration sort("Total sort");
  TotalDuration answer("Total create answer");
  for (string current_query; getline(query_input, current_query); ) {
    std::vector<size_t> indexes;
    std::array<size_t, 50000> docs;
    {
      ADD_DURATION(parse)
      for (const auto& word : SplitIntoWords(current_query)) {
        auto vec = index.Lookup(word);
        for (const auto& [docid, count] : vec) {
          docs[docid] += count;
          indexes.push_back(docid);
        }
      }
    }

    std::vector<pair<size_t, size_t>> search_results;
    {
      ADD_DURATION(move)
      search_results.reserve(indexes.size());
      for (const auto& id : indexes) {
        search_results.emplace_back(std::make_pair(id, docs[id]));
      }
    }
    {
      ADD_DURATION(sort)
      std::sort(
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
    }

    {
      ADD_DURATION(answer)
      search_results_output << current_query << ':';
      for (auto [docid, hitcount] : Head(search_results, 5)) {
        search_results_output << " {"
                              << "docid: " << docid << ", "
                              << "hitcount: " << hitcount << '}';
      }
      search_results_output << endl;
    }

  }
}

void InvertedIndex::Add(string &&document) {
  docs.push_back(std::move(document));
  const size_t docid = docs.size() - 1;
  for (const auto& word : SplitIntoWords(docs.back())) {
    auto& vec_pair = freq_index[word];
    if (!vec_pair.empty() && vec_pair.back().first == docid) {
      vec_pair.back().second += 1;
    }
    else {
      vec_pair.emplace_back(docid, 1);
    }
  }
}

std::vector<std::pair<size_t, size_t>> InvertedIndex::Lookup(string_view word) const {
  if (auto it = freq_index.find(word); it != freq_index.end()) {
    return it->second;
  } else {
    return {};
  }
}