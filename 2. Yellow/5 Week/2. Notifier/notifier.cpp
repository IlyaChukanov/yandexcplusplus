//
// Created by ilya on 15.09.2019.
//

#include <string>
#include <iostream>


class INotifier {
 public:
  virtual void Notify(const std::string& message) = 0;
};

class SmsNotifier : public INotifier {
 public:
  SmsNotifier(const std::string& number) : number_(number) {}

  void Notify(const std::string& message) override {
    SendSms(number_, message);
  }
 private:
  std::string number_;
};

class EmailNotifier : public INotifier {
 public:
  EmailNotifier(const std::string& email) : email_(email) {}

  void Notify(const std::string& message) override {
    SendEmail(email_, message);
  }
 private:
  std::string email_;
};