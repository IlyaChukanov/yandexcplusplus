#include "../../../MyUtils/MyTestFramework/TestFramework.h"
#include "../../../MyUtils/Profiler/profiler.h"


#include "search_server.h"
#include "parse.h"

#include <algorithm>
#include <iterator>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <random>
#include <thread>

using namespace std;

void TestFunctionality(
  const vector<string>& docs,
  const vector<string>& queries,
  const vector<string>& expected
) {
  std::cerr << "------------" << std::endl;
  istringstream docs_input(Join('\n', docs));
  istringstream queries_input(Join('\n', queries));
  ostringstream queries_output;
  {
    SearchServer srv;
    {
      LOG_DURATION("UpdateDocumentBase");
      srv.UpdateDocumentBase(docs_input);
    }
    {
      LOG_DURATION("AddQueriesStream");
      srv.AddQueriesStream(queries_input, queries_output);
    }
  }


  const string result = queries_output.str();
  const auto lines = SplitBy(Strip(result), '\n');
  ASSERT_EQUAL(lines.size(), expected.size());
  for (size_t i = 0; i < lines.size(); ++i) {
    ASSERT_EQUAL(lines[i], expected[i]);
  }
}

void TestSerpFormat() {
  const vector<string> docs = {
    "london is the capital of great britain",
    "i am travelling down the river"
  };
  const vector<string> queries = {"london", "the"};
  const vector<string> expected = {
    "london: {docid: 0, hitcount: 1}",
    Join(' ', vector{
      "the:",
      "{docid: 0, hitcount: 1}",
      "{docid: 1, hitcount: 1}"
    })
  };

  TestFunctionality(docs, queries, expected);
}

void TestTop5() {
  const vector<string> docs = {
    "milk a",
    "milk b",
    "milk c",
    "milk d",
    "milk e",
    "milk f",
    "milk g",
    "water a",
    "water b",
    "fire and earth"
  };

  const vector<string> queries = {"milk", "water", "rock"};
  const vector<string> expected = {
    Join(' ', vector{
      "milk:",
      "{docid: 0, hitcount: 1}",
      "{docid: 1, hitcount: 1}",
      "{docid: 2, hitcount: 1}",
      "{docid: 3, hitcount: 1}",
      "{docid: 4, hitcount: 1}"
    }),
    Join(' ', vector{
      "water:",
      "{docid: 7, hitcount: 1}",
      "{docid: 8, hitcount: 1}",
    }),
    "rock:",
  };
  TestFunctionality(docs, queries, expected);
}

void TestHitcount() {
  const vector<string> docs = {
    "the river goes through the entire city there is a house near it",
    "the wall",
    "walle",
    "is is is is",
  };
  const vector<string> queries = {"the", "wall", "all", "is", "the is"};
  const vector<string> expected = {
    Join(' ', vector{
      "the:",
      "{docid: 0, hitcount: 2}",
      "{docid: 1, hitcount: 1}",
    }),
    "wall: {docid: 1, hitcount: 1}",
    "all:",
    Join(' ', vector{
      "is:",
      "{docid: 3, hitcount: 4}",
      "{docid: 0, hitcount: 1}",
    }),
    Join(' ', vector{
      "the is:",
      "{docid: 3, hitcount: 4}",
      "{docid: 0, hitcount: 3}",
      "{docid: 1, hitcount: 1}",
    }),
  };
  TestFunctionality(docs, queries, expected);
}

void TestRanking() {
  const vector<string> docs = {
    "london is the capital of great britain",
    "paris is the capital of france",
    "berlin is the capital of germany",
    "rome is the capital of italy",
    "madrid is the capital of spain",
    "lisboa is the capital of portugal",
    "bern is the capital of switzerland",
    "moscow is the capital of russia",
    "kiev is the capital of ukraine",
    "minsk is the capital of belarus",
    "astana is the capital of kazakhstan",
    "beijing is the capital of china",
    "tokyo is the capital of japan",
    "bangkok is the capital of thailand",
    "welcome to moscow the capital of russia the third rome",
    "amsterdam is the capital of netherlands",
    "helsinki is the capital of finland",
    "oslo is the capital of norway",
    "stockgolm is the capital of sweden",
    "riga is the capital of latvia",
    "tallin is the capital of estonia",
    "warsaw is the capital of poland",
  };

  const vector<string> queries = {"moscow is the capital of russia"};
  const vector<string> expected = {
    Join(' ', vector{
      "moscow is the capital of russia:",
      "{docid: 7, hitcount: 6}",
      "{docid: 14, hitcount: 6}",
      "{docid: 0, hitcount: 4}",
      "{docid: 1, hitcount: 4}",
      "{docid: 2, hitcount: 4}",
    })
  };
  TestFunctionality(docs, queries, expected);
}

void TestBasicSearch() {
  const vector<string> docs = {
    "we are ready to go",
    "come on everybody shake you hands",
    "i love this game",
    "just like exception safety is not about writing try catch everywhere in your code move semantics are not about typing double ampersand everywhere in your code",
    "daddy daddy daddy dad dad dad",
    "tell me the meaning of being lonely",
    "just keep track of it",
    "how hard could it be",
    "it is going to be legen wait for it dary legendary",
    "we dont need no education"
  };

  const vector<string> queries = {
    "we need some help",
    "it",
    "i love this game",
    "tell me why",
    "dislike",
    "about"
  };

  const vector<string> expected = {
    Join(' ', vector{
      "we need some help:",
      "{docid: 9, hitcount: 2}",
      "{docid: 0, hitcount: 1}"
    }),
    Join(' ', vector{
      "it:",
      "{docid: 8, hitcount: 2}",
      "{docid: 6, hitcount: 1}",
      "{docid: 7, hitcount: 1}",
    }),
    "i love this game: {docid: 2, hitcount: 4}",
    "tell me why: {docid: 5, hitcount: 2}",
    "dislike:",
    "about: {docid: 3, hitcount: 2}",
  };
  TestFunctionality(docs, queries, expected);
}

void TestHighLoad() {
  std::cerr << "HIGH LOAD TEST 1" << std::endl;
  std::string word1 = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
  std::string word2 = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
  std::string word3 = "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc";
  std::string word4 = "dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd";
  std::string word5 = "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee";

  vector<string> docs;
  const size_t DOCS_COUNT = 50000;
  const size_t DOCS_WORD_COUNT = 10;
  for (size_t i = 0; i < DOCS_COUNT; ++i) {
    std::string docs_str;
    for (size_t j = 0; j < DOCS_WORD_COUNT / 5; ++j) {
      docs_str = word1 + " " + word2 + " "  + word3 + " "  + word4 + " "  + word5;
    }
    docs.push_back(docs_str);
  }

  const size_t QUERIES_COUNT = 50000;
  const size_t QUERIES_WORD_COUNT = 10;
  vector<string> queries;
  for (size_t i = 0; i < QUERIES_COUNT; ++i) {
    std::string q_str;
    for (size_t j = 0; j < QUERIES_WORD_COUNT / 2; ++j) {
      q_str = word2 + " "  +  word4;
    }
    queries.push_back(q_str);
  }

  std::cerr << "HIGH LOAD TEST" << std::endl;
  istringstream docs_input(Join('\n', docs));
  istringstream queries_input(Join('\n', queries));

  SearchServer srv;
  {
    LOG_DURATION("UpdateDocumentBase");
    srv.UpdateDocumentBase(docs_input);
  }
  ostringstream queries_output;
  {
    LOG_DURATION("AddQueriesStream");
    srv.AddQueriesStream(queries_input, queries_output);
  }
}

void TestHighLoad1() {
  std::cerr << "HIGH LOAD TEST" << std::endl;
  vector<string> docs;
  const size_t DOCS_COUNT = 10'000;
  for (size_t i = 0; i < DOCS_COUNT; ++i) {
    docs.emplace_back("Hello this is firstaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa line of really high load docs");
    docs.emplace_back("Hello this is secondaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa line of really high load docs");
    docs.emplace_back("Hello this is thirdaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa line. I think it must be another type");
    docs.emplace_back("Hello this is fouraaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa line. Do you likeaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa Yangleaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa? I am likeaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa it");
    docs.emplace_back("Hello this is fiveaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa line. Do you likeaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa Goondexaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa? He is really cool!");
  }
  const vector<string> queries = {
      "firstaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
      "secondaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
      "thirdaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
      "fouraaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
      "fiveaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
      "Yangleaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
      "Goondexaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
      "likeaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
      "He is really cool!",
      "I am likeaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa it"
  };

  istringstream docs_input(Join('\n', docs));
  istringstream queries_input(Join('\n', queries));

  SearchServer srv;
  {
    LOG_DURATION("UpdateDocumentBase");
    srv.UpdateDocumentBase(docs_input);
  }
  ostringstream queries_output;
  {
    LOG_DURATION("AddQueriesStream");
    srv.AddQueriesStream(queries_input, queries_output);
  }

  const string result = queries_output.str();
  const auto lines = SplitBy(Strip(result), '\n');
}

void TestSearchServer(std::vector<std::pair<istringstream, ostringstream>> & streams) {
  LOG_DURATION("Total");
  SearchServer srv(streams.front().first);
  for (auto& [input, output] :
      IteratorRange(begin(streams) + 1, end(streams))) {
    if (!output) {
      srv.UpdateDocumentBase(input);
    } else {
      srv.AddQueriesStream(input, output);
    }
  }
}


void TestParallel() {
  std::cerr << "HIGH PARALLEL" << std::endl;
  std::string word1 = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
  std::string word2 = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
  std::string word3 = "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc";
  std::string word4 = "dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd";
  std::string word5 = "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee";

  vector<string> docs;
  const size_t DOCS_COUNT = 10000;
  const size_t DOCS_WORD_COUNT = 10;
  for (size_t i = 0; i < DOCS_COUNT; ++i) {
    std::string docs_str;
    for (size_t j = 0; j < DOCS_WORD_COUNT / 5; ++j) {
      docs_str = word1 + " " + word2 + " "  + word3 + " "  + word4 + " "  + word5;
    }
    docs.push_back(docs_str);
  }

  const size_t QUERIES_COUNT = 1000;
  const size_t QUERIES_WORD_COUNT = 10;
  vector<string> queries;
  for (size_t i = 0; i < QUERIES_COUNT; ++i) {
    std::string q_str;
    for (size_t j = 0; j < QUERIES_WORD_COUNT / 2; ++j) {
      q_str = word2 + " "  +  word4;
    }
    queries.push_back(q_str);
  }

  const size_t streams_pair = 2;
  ostringstream empty;
  istringstream input_docs(Join('\n', docs));
  std::vector<std::pair<istringstream, ostringstream>> streams;
  streams.emplace_back(std::move(input_docs), std::move(empty));
  for (size_t i = 0; i < streams_pair; ++i) {
    istringstream queries_input(Join('\n', queries));
    ostringstream output;
    streams.emplace_back(std::move(queries_input), std::move(output));
  }
  TestSearchServer(streams);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestTop5);
  RUN_TEST(tr, TestHitcount);
  RUN_TEST(tr, TestRanking);
  RUN_TEST(tr, TestBasicSearch);
  RUN_TEST(tr, TestHighLoad);
  RUN_TEST(tr, TestHighLoad1);
  RUN_TEST(tr, TestSerpFormat);
  RUN_TEST(tr, TestParallel);
}
