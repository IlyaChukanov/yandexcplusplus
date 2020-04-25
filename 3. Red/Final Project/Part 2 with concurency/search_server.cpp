#include "search_server.h"
#include "iterator_range.h"

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

SearchServer::~SearchServer() {
  for (auto& f : futures_) {
    f.get();
  }
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
  InvertedIndex new_index;
  for (string current_document; getline(document_input, current_document); ) {
    new_index.Add(move(current_document));
  }
  {
    std::lock_guard<std::mutex> lock(index_mutex_);
    std::swap(new_index, index_);
  }
}

void SearchServer::AddQueriesStream(istream& query_input, ostream& search_results_output) {
  // Доделать проверку на ограничение future и ожидание пока future не освободится?
  auto future = [&query_input, &search_results_output, this]() {
    std::vector<size_t> docs;
    std::vector<size_t> ind;
    docs.resize(50000);
    ind.resize(50000);
    for (string current_query; getline(query_input, current_query); ) {
      size_t curr_ind = 0;
      for (const auto& word : SplitIntoWords(current_query)) {
        std::vector<std::pair<size_t, size_t>> vec;
        {
          std::lock_guard<std::mutex> lock(index_mutex_);
          vec = index_.Lookup(word);
        }
        for (const auto& [docid, count] : vec) {
          if (docs[docid] == 0) {
            ind[curr_ind++] = docid;
          }
          docs[docid] += count;
        }
      }

      std::vector<std::pair<size_t, size_t>> search_result;
      for (size_t docid = 0; docid < curr_ind; ++docid) {
        size_t count = 0;
        size_t id = 0;
        std::swap(count, docs[ind[docid]]);
        std::swap(id, ind[docid]);
        search_result.emplace_back(id, count);
      }

      const size_t ANSWERS_COUNT = 5;
      std::partial_sort(
          begin(search_result),
          begin(search_result) + std::min<size_t>(ANSWERS_COUNT, search_result.size()),
          end(search_result),
          [](pair<size_t, size_t> lhs, pair<size_t, size_t> rhs) {
            int64_t lhs_docid = lhs.first;
            auto lhs_hit_count = lhs.second;
            int64_t rhs_docid = rhs.first;
            auto rhs_hit_count = rhs.second;
            return make_pair(lhs_hit_count, -lhs_docid) > make_pair(rhs_hit_count, -rhs_docid);
          }
      );

      search_results_output << current_query << ':';
      for (auto [docid, hitcount] : Head(search_result, ANSWERS_COUNT)) {
        search_results_output << " {"
                              << "docid: " << docid << ", "
                              << "hitcount: " << hitcount << '}';
      }
      search_results_output << endl;
    }
  };
  futures_.push_back(std::async(future));
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