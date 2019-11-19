#include "../../../MyUtils/MyTestFramework/TestFramework.h"
#include <functional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <stack>
#include <iostream>

using namespace std;


struct Email {
  Email() = default;
  Email(std::string from_, std::string to_, std::string body_) : from(std::move(from_)),
                                                                 to(std::move(to_)),
                                                                 body(std::move(body_)) {}
  string from;
  string to;
  string body;
};

class Worker {
 public:
  virtual ~Worker() = default;
  virtual void Process(unique_ptr<Email> email) = 0;

  virtual void Run() {
    // только первому worker-у в пайплайне нужно это имплементировать
    throw logic_error("Unimplemented");
  }

  void SetNext(unique_ptr<Worker> next) {
    next_exec_ = std::move(next);
  }
 protected:
  // реализации должны вызывать PassOn, чтобы передать объект дальше
  // по цепочке обработчиков
  void PassOn(unique_ptr<Email> email) const {
    if (next_exec_) {
      next_exec_->Process(std::move(email));
    }
  }
 private:
  std::unique_ptr<Worker> next_exec_;
};


class Reader : public Worker {
 public:
  explicit Reader(istream& read_msg) : messages_(read_msg) {}

  void Process(unique_ptr<Email> email) override {
    throw logic_error("Unimplemented");
  }

  void Run() override {
    while(messages_) {
      std::string from, to, body;
      std::getline(messages_, from);
      std::getline(messages_, to);
      std::getline(messages_, body);
      if (messages_) {
        auto email = std::make_unique<Email>(from, to, body);
        PassOn(std::move(email));
      }
    }
  }

 private:
  istream& messages_;
};


class Filter : public Worker {
 public:
  using Function = function<bool(const Email&)>;

  explicit Filter(Function func) : func_(std::move(func)) {}

  void Process(unique_ptr<Email> email) override {
    if (func_(*email)) {
      PassOn(std::move(email));
    }
  }

 private:
  Function func_;
};


class Copier : public Worker {
 public:
  explicit Copier(const std::string& addr) : another_addres_(addr) {}

  void Process(unique_ptr<Email> email) override {
    if (email->to != another_addres_) {
      Email email_obj = {email->from, another_addres_, email->body};
      PassOn(std::move(email));
      PassOn(make_unique<Email>(email_obj));
    }
    else {
      PassOn(std::move(email));
    }
  }

 private:
  std::string another_addres_;
};


class Sender : public Worker {
 public:
  explicit Sender(ostream& read_msg) : output_(read_msg) {}

  void Process(unique_ptr<Email> email) override {
    output_ << email->from << std::endl
            << email->to << std::endl
            << email->body << std::endl;
    PassOn(std::move(email));
  }

 private:
  ostream& output_;
};


// реализуйте класс
class PipelineBuilder {
public:
  // добавляет в качестве первого обработчика Reader
  explicit PipelineBuilder(istream& in) {
    workers_.push_back(make_unique<Reader>(in));
  }

  PipelineBuilder& FilterBy(Filter::Function filter) {
    workers_.push_back(make_unique<Filter>(move(filter)));
    return *this;
  }

  PipelineBuilder& CopyTo(string recipient) {
    workers_.push_back(make_unique<Copier>(move(recipient)));
    return *this;
  }

  PipelineBuilder& Send(ostream& out) {
    workers_.push_back(make_unique<Sender>(out));
    return *this;
  }

  unique_ptr<Worker> Build() {
    for (size_t i = workers_.size() - 1; i > 0; --i) {
      workers_[i - 1]->SetNext(move(workers_[i]));
    }
    return move(workers_[0]);
  }

 private:
  std::vector<std::unique_ptr<Worker>> workers_;
};


void TestSanity() {
  string input = (
    "erich@example.com\n"
    "richard@example.com\n"
    "Hello there\n"

    "erich@example.com\n"
    "ralph@example.com\n"
    "Are you sure you pressed the right button?\n"

    "ralph@example.com\n"
    "erich@example.com\n"
    "I do not make mistakes of that kind\n"
  );
  istringstream inStream(input);
  ostringstream outStream;

  PipelineBuilder builder(inStream);
  builder.FilterBy([](const Email& email) {
    return email.from == "erich@example.com";
  });
  builder.CopyTo("richard@example.com");
  builder.Send(outStream);
  auto pipeline = builder.Build();

  pipeline->Run();

  string expectedOutput = (
    "erich@example.com\n"
    "richard@example.com\n"
    "Hello there\n"

    "erich@example.com\n"
    "ralph@example.com\n"
    "Are you sure you pressed the right button?\n"

    "erich@example.com\n"
    "richard@example.com\n"
    "Are you sure you pressed the right button?\n"
  );
  auto a = outStream.str();
  ASSERT_EQUAL(expectedOutput, outStream.str());
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSanity);
  return 0;
}
