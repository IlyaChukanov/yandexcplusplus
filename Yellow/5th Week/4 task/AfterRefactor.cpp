//
// Created by ilya on 15.09.2019.
//

#include <iostream>
#include <string>
#include <vector>

class Human {
 public:
  explicit Human(const std::string& name, const std::string& type = "human") : name_(name), type_(type) {}

  virtual void Walk(const std::string& destination) const = 0;

  std::string GetName() const {
    return name_;
  }
  std::string GetType() const {
    return type_;
  }

 protected:
  const std::string name_;
  const std::string type_;
};

class Student : public Human {
 public:
  Student(const std::string& name, const std::string& fav_song)
  : Human(name, "student")
  , favourite_song_(fav_song) {}

  void Walk(const std::string& destination) const override {
    std::cout << "Student: " << name_ << " walks to: " << destination << std::endl;
    SingSong();
  }

  void Learn() const {
    std::cout << "Student: " << name_ << " learns" << std::endl;
  }

  void SingSong() const {
    std::cout << "Student: " << name_ << " sings a song: " << favourite_song_ << std::endl;
  }

 private:
  const std::string favourite_song_;
};

class Teacher : public Human {
 public:
  Teacher(const std::string& name, const std::string& subject)
  : Human(name, "teacher")
  , subject_(subject) {}

  void Walk(const std::string& destination) const override {
    std::cout << "Teacher: " << name_ << " walks to: " << destination << std::endl;
  }

  void Teach() const {
    std::cout << "Teacher: " << name_ << " teaches: " << subject_ << std::endl;
  }

 private:
  const std::string subject_;
};

class Policeman : public Human {
 public:

  explicit Policeman(const std::string& name) : Human(name, "policeman") {}

  void Walk(const std::string& destination) const override {
    std::cout << "Policeman: " << name_ << " walks to: " << destination << std::endl;
  }

  void Check(const Human& h) const {
    std::cout << "Policeman: " << name_ << " checks " << h.GetType() << ". " << h.GetType() << "'s name is: " << h.GetName() << std::endl;
  }

};

void VisitPlaces(Human& h, const std::vector<std::string>& places) {
  for (const auto& p : places) {
    h.Walk(p);
  }
}

int main() {
  Teacher t("Jim", "Math");
  t.Teach();
  Student s("Ann", "We will rock you");
  s.SingSong();
  s.Learn();
  Policeman p("Bob");

  VisitPlaces(t, {"Moscow", "London"});
  p.Check(s);
  VisitPlaces(s, {"Moscow", "London"});
  return 0;
}
